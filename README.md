This is the initial Github repository for the Reflow Oven

<strong>Infrawave Oven:</strong>

Recently I viewed <a href="http://www.youtube.com/watch?v=NCGzKDTFBSQ" target="_blank">Ben Heck's Reflow Oven Video</a> and was pretty impressed with the time response of this oven. Ben uses a SSR and an external Arduino based controller to run his oven.  I really like this standalone solution. Other research revealed <a href="http://hobbybotics.com/projects/hobbybotics-reflow-controller-v8-03/" target="_blank">Hobbybotics Reflow Controller</a> and this is a very impressive build.  Lots of background on PID and detailed build analysis is provided.  This controller appears to be computer controlled and requires an external control box. Coupled with the handy <a href="http://www.rocketscream.com/wiki/index.php/Reflow_Controller_Shield_%28Arduino_Compatible%29">Rocket Scream Reflow Controller Shield</a> and Arduino source code I though I would  come up with a new and novel solution to control the <a href="http://www.amazon.com/gp/product/B001AX8F4Y?ref_=sr_1_1&amp;qid=1367115903&amp;sr=8-1&amp;keywords=infrawave%20oven" target="_blank">Black and Decker Infrawave Oven</a> as shown below.

<img class="aligncenter" alt="Black and Decker Infrawave Oven" src="http://ecx.images-amazon.com/images/I/41zCQsaULjL.jpg" width="600" />

<strong>Why do this?:</strong>
I am in the process of releasing a whole line of led matrix products. Part of this business decision has been to bring in as much in-house manufacturing as possible. We have already received and installed our Quad IVc pick and place. This has pretty much tapped our current budget and I needed another method that would provide for a proper reflow profile of the boards I am working with. The hot plate method has worked well for us, but I have noticed a number of cold solder joints on our boards recently and this has been secretly driving me crazy. In order to properly reflow solder paste it is required to properly heat the solder to get good results as shown below. Based on my part failures I was clearly not achieving the recommend profile.
<p style="text-align: center;"><a href="http://ohararp.com/wp/wp-content/uploads/2013/04/kester21.png" target="_blank"><img class="aligncenter size-full wp-image-463" alt="kester2" src="http://ohararp.com/wp/wp-content/uploads/2013/04/kester21.png" width="600" height="434" /></a></p>
I ran some tests evaluate what the reflow profile of the hotplate I was using really was and was pretty disappointed.  The pcb temperature never rose above 200C.  I previously attempted using a cheap reflow oven and was not happy with the performance of this unit either.  It was an Oster unit with standard resistive elements.  It took forever to heat up.  I ran some "full on" tests on this unit to characterize its poor performance. The unit has incredibly poor rise rates of less than 0.5 C/s and the unit eventually reaches 227 deg C but only after 600 seconds. There is not enough overhead for this unit to control a full ramp reflow profile for a lead solder, much less a lead free solder.
<p style="text-align: center;"><a href="http://ohararp.com/wp/wp-content/uploads/2013/04/OsterOn.jpg" target="_blank"><img class="aligncenter  wp-image-449" alt="OsterOn" src="http://ohararp.com/wp/wp-content/uploads/2013/04/OsterOn.jpg" width="600" /></a></p>
<strong>Teardown:</strong>

Based upon Ben Heck's video I placed an order on Amazon for this oven.  For $106 and Amazon Prime shipping I had the oven here in 2 days.  Can't beat that!  I looked around in town briefly and was unable to find this unit in any of the big box stores.  It seems there was timewhere it was hard to source these ovens for awhile.  Amazon seems to have made this alot easier these days.  Once the unit I arrived I tore down the unit much like Ben did.  When I got down to it I found two different pcbs that were used to operate the oven.  The first was the main relay board that controls the top and bottom elements as shown below.
<p style="text-align: center;"><a href="http://ohararp.com/wp/wp-content/uploads/2013/04/OrginalControlBoards.jpg" target="_blank"><img class=" wp-image-410 aligncenter" alt="OrginalControlBoards" src="http://ohararp.com/wp/wp-content/uploads/2013/04/OrginalControlBoards.jpg" width="600" height="225" /></a></p>
They are one sided pcbs that very effectively cram a combination of smd and thru hole parts to control the oven. There is no temperature control built into this oven. Everything is based upon predetermined profiles. Pretty impressive if you ask me. The first board is a menu control board that communicates the users intent to the power management board. This unit uses a Holtek IC anc controls a small lcd, numerous button inputs, and several leds. The second board, contains the non-isolated power supply, 2 power relays (top and bottom elements), and the door switch input. Both are neatly tied together with a wire harness. All power input and elements are neatly wired with blade connectors. Altogether it quickly appears that by replacing these 2 boards I could effectively "hack" the oven with my own controller and input boards to make for a neat and professional package that uses the units existing LCD view window and button pads.

With this in mind I decided to create a wish list of features that I would want:

<span style="text-decoration: underline;">Control Board:</span><strong>
</strong><span style="font-size: 13px; line-height: 19px;">1. Leverage existing PID code
2.</span> Oled 128x64 Display
3. Serial Bootloading<em id="__mceDel" style="font-size: 13px; line-height: 19px;"><em id="__mceDel"><em id="__mceDel">
</em></em></em>4. Wireless Data Output

<span style="text-decoration: underline;">Power Board:</span>
1. Isolated 5V Power supply
2. Triac Based Element Control
3. Door Switch Input (not implemented in code)

Below you can see the schematic of the control board that fits these requirements.  It uses an Atmega 32U4 running at 8MHz and 3.3V.  Running at 3.3V will allow the uController to talk directly to the SSD1306 OLED without logic level translation.  
<p style="text-align: center;"><a href="https://raw.github.com/ohararp/ReflowOven/master/PcbDesignFiles/ReflowControllerC.jpg" target="_blank"><img class="aligncenter  wp-image-423" alt="ReflowControllerA" src="https://raw.github.com/ohararp/ReflowOven/master/PcbDesignFiles/ReflowControllerC.jpg" width="600" height="421" /></a></p>
<p style="text-align: left;">The power control board provides for a nice isolated input using the <a href="http://www.digikey.com/product-detail/en/FSK-S5-5U/102-1470-ND/1016678">FSK-S5-5U</a> from CUI/Digikey.  To control the top and bottom elments 2 25 A BTAT25 smd triacs were used with MOC3032M zero cross detecting optocouplers. </p>
<p style="text-align: left;"><a href="https://raw.github.com/ohararp/ReflowOven/master/PcbDesignFiles/ReflowPowerC.jpg" target="_blank"><img class="aligncenter  wp-image-424" alt="ReflowPowerA" src="https://raw.github.com/ohararp/ReflowOven/master/PcbDesignFiles/ReflowPowerC.jpg" width="600" /></a></p>
<strong>PCB Design:</strong>

One of the great parts of our Stencil business is having access to the 45W CO2 Laser for cutting of plastics.  Armed with some calipers I proceeded to reverse engineer the shapes of the pcbs, mounting locations, and button and led locations.  Using Corel Draw I came up with the shapes you see here.  Cutting these on the laser allowed me to make sure the boards would fit prior to designing and ordering pcbs that wouldn't fit.  Good thing since it took 4 or 5 times to make sure everything would line up.
<p style="text-align: center;"><a href="http://ohararp.com/wp/wp-content/uploads/2013/04/PcbReverseEngineer.jpg" target="_blank"><img class="aligncenter  wp-image-420" alt="PcbReverseEngineer" src="http://ohararp.com/wp/wp-content/uploads/2013/04/PcbReverseEngineer.jpg" width="600" /></a></p>
Using these designs I was able to create the pcb board outlines and properly place all of the required parts.  I use a <a href="http://www.labcenter.com/index.cfm">Labcetner's Proteus Design Package</a> to do my board layout.  Luckily, I could leverage a number of known circuits and working designs for this project and layout went quickly.  Care was taken to make sure that all the parts would be mounted in the oven without having to modify the interior components.  Knowing that this was going to be a quick turn project I knew ahead of time that I was going to be using bare pcbs (no soldermask).  I made sure to keep large spacing between pads and the ground and power planes.  The results of this effort can be seen below.
<p style="text-align: center;"><a href="https://raw.github.com/ohararp/ReflowOven/master/PcbDesignFiles/PanelLayout.jpg" target="_blank"><img class="aligncenter  wp-image-428" alt="BareBonesCombo" src="https://raw.github.com/ohararp/ReflowOven/master/PcbDesignFiles/PanelLayout.jpg" width="600" /></a></p>
<strong>Assembly:</strong>

Due to the one off nature of this control board I performed most of the assembly by hand soldering components into place.  Here you can see the power board assembled. If you click on the image the labels for all the wiring cables can be seen.
<p style="text-align: center;"><a href="http://ohararp.com/wp/wp-content/uploads/2013/04/AssembledControlBoard.jpg" target="_blank"><img class="aligncenter  wp-image-435" alt="AssembledControlBoard" src="http://ohararp.com/wp/wp-content/uploads/2013/04/AssembledControlBoard.jpg" width="600" /></a></p>
Here we have the 2 boards assembled.  The connection between the 2 boards was made with some 5 cm 3 wire servo cables.
<p style="text-align: center;"><a href="http://ohararp.com/wp/wp-content/uploads/2013/04/AssembledBoards.jpg" target="_blank"><img class="aligncenter  wp-image-438" alt="AssembledBoards" src="http://ohararp.com/wp/wp-content/uploads/2013/04/AssembledBoards.jpg" width="600" /></a></p>
<p style="text-align: left;">With the boards assembled the oven can be repackaged into its original setup. This image shows the use of the oled display and its visibility through the existing view window of the oven.</p>
<p style="text-align: center;"><a href="http://ohararp.com/wp/wp-content/uploads/2013/04/IMG_0994.jpg"><img class="aligncenter  wp-image-439" alt="IMG_0994" src="http://ohararp.com/wp/wp-content/uploads/2013/04/IMG_0994.jpg" width="600" /></a></p>
<strong>Initial Testing:</strong>

<span style="font-size: 13px; line-height: 19px;"><span style="text-decoration: underline;">Full On</span> - With the oven reassembled a couple of tests were run to determine the performance of the oven.  The first was a full on test to see the max potential of this setup.  The top graph shows the temperature of the unit over time.  This shows a max temp acheived of 300C.  I think the oven could have gone higher but the smoking pcb that was used as a dummy prevented this.  The burnt fiberglass smell has only gone away after a week. The bottom graph is the derivative of the time data and allows us to determine the rise and fall rate of the oven.  Since the data was sampled on the second this calculation was pretty easy to do.  Basically, the unit is able to handle 1+ deg/s rise rate and greater than this on the cool down.  Based off this data it is clear that this oven offers a lot of potential.</span>
<p style="text-align: center;"><a href="http://ohararp.com/wp/wp-content/uploads/2013/04/FullOn.jpg" target="_blank"><img class="aligncenter  wp-image-443" alt="FullOn" src="http://ohararp.com/wp/wp-content/uploads/2013/04/FullOn.jpg" width="600" /></a></p>
<p style="text-align: left;"><strong>Actual Reflow Testing:</strong>
With the Wireless capability of the XBEE the output of data is extremely easy. Implementing this was a good option and frees up my pc for other things. The image below shows how smoothly things went. I reused the existing piezo buzzer from the original pcbs and added some code to have the unit beep at me after the max reflow temperature is reached. At this time the door is opened and the unit is allowed to cool.</p>
<a href="http://ohararp.com/wp/wp-content/uploads/2013/04/Control3.jpg" target="_blank"><img class="aligncenter  wp-image-458" alt="Control3" src="http://ohararp.com/wp/wp-content/uploads/2013/04/Control3.jpg" width="600" /></a>

<a href="http://ohararp.com/wp/wp-content/uploads/2013/04/MatrixReflow.jpg"><img class="aligncenter size-large wp-image-479" alt="MatrixReflow" src="http://ohararp.com/wp/wp-content/uploads/2013/04/MatrixReflow-1024x768.jpg" width="600" /></a>

<strong>Startup Video:</strong>

http://youtu.be/EhMYh9KmT7Y

<strong>Soak Video:</strong>

http://youtu.be/6kIh7mlD550

<strong style="font-size: 13px; line-height: 19px;">ReflowVideo:</strong><em id="__mceDel" style="font-size: 13px; line-height: 19px;"></em>

http://youtu.be/iLTvkUca_Ug

<strong style="font-size: 13px; line-height: 19px;">Cool Down:</strong><em id="__mceDel" style="font-size: 13px; line-height: 19px;"></em>

http://youtu.be/FBCPu6zQ0Fg

<strong style="font-size: 13px; line-height: 19px;">Final Inspection:</strong><em id="__mceDel" style="font-size: 13px; line-height: 19px;"></em>

http://youtu.be/jnSopUsd7fs

<strong>Conclusion:</strong>
Overall, this was a great project and I am confident that this oven can be used regularly for reflow.  Having the controlled profile allows me to do other things while pcbs are reflowing. I am glad I could leverage the work of the open source community and come up with this great project.

The bulk of the control code was a modification of the the <a href="https://github.com/rocketscream/Reflow-Oven-Controller/blob/master/reflowOvenController.ino">Rocket Scream Control Code</a>.  The code I used can be found on <a href="https://github.com/ohararp/ReflowOvenA">Github</a>.
