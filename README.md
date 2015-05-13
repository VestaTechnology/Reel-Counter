#Reel Counter
Inventory time was fast approaching.  We had the mechanical parts of a reel counter, but we were lacking the electronics.  Luckily, Mercury 16 could preform the role of the electronics! 

The reel counter has a sprocket gear with holes in it that the reels of parts are passed over.  Two pairs of infrared LEDs and photodiodes are held in place with blocks of wood.  The two pairs are arranged so they create a [quadrature signal][qs] as the sprocket gear is turned by parts. 

[qs]: https://en.wikipedia.org/wiki/Linear_encoder#Output_signal_formats

![Reel Counter and Sensors] (https://github.com/VestaTechnology/Reel-Counter/blob/master/Reel4Fig2.jpg  "Reel Counter and Sensors")

When the gear is turning, the voltage across the photodiodes looks a lot more like a sine wave than a square wave.  In order for the signal to be used for an interrupt on change, it needed some conditioning.  I fed the signals into a 2-channel comparator with simple dividers for reference voltages.  To make the output a clean square wave, I added hysteresis to each comparator channel.  

This [webpage][volt comp] has great information about voltage comparators and describes how to add hysteresis.

[volt comp]: http://home.cogeco.ca/~rpaisley4/Comparators.html

The only circuit elements needed besides a Mercury 16 were some connections to control the contrast and back-lighting of an LCD, and 3 buttons to reset the count and change how many holes per part the reel has.

![Circuit Schematic](https://github.com/VestaTechnology/Reel-Counter/blob/master/circuit.jpg "Circuit Schematic")

###Understanding the Code
This project was completed when Vesta Technology was using MPLAB v8, so I'll only upload the source without and project files here.

__Interface__ contains routines to interface the Mercury 16 with the three buttons and the quadrature signal generated by the reel counter.

__Interrupt__ contains the Interrupt Service Routine (ISR) and related functions.  The ISR is triggered by interrupt-on-change pins to decode the quadrature signal and timer interrupts to debounce switches.  The debounce routines were found on [Jack Ganssle's website][debounce].  Thank you very much!

[debounce]: http://www.ganssle.com/debouncing-pt2.htm

__LCD__ module provides and easy interface between Mercury and an LCD.  It is a generic module that can be used on any Mercury project to drive an LCD with a Hitachi 44780 controller chip.

__Main.c__ only contains a few function calls to initialize communications and an infinite while loop that calls the updateLCD function if the update flag has been set by the ISR.

![Final Product](https://github.com/VestaTechnology/Reel-Counter/blob/master/Reel6Fig1.jpg "Final Product")

Next time you need a piece of equipment, I hope this can serve as inspiration to build it instead of buy it.  It was a great learning experience for me and does a great job showing off the capabilities of Mercury.  I haven't yet been able to crank the counter fast enough to cause timing issues.  Please share what you do with this project and submit a pull request with any changes and improvements!  As always, you can submit an issue, [contact us][contact] or send us an [email][mail] if you need help or have questions.

[contact]: https://www.vestatech.com/support/contact-us/
[mail]: mailto:support@vestatech.com?subj=Github/Mercury
