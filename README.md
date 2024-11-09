# blockfinder
Find bit isolated empty regions on OTP ROM images for reuse.

---------------------------

ABOUT:


This tool finds ranges of un-programmed memory space on PROMS so that areas of them can be re-used by locking address bits to constrain your storage range.

This tool finds the largest blocks first, and then finds successively smaller blocks down to 16 bytes (It could go further, but I dont see the point)

example output:


> This program finds empty blocks on ROM images. 
> Written by Rue Mohr 2024 
>
>File image size is 15 bits
>
>2 kbits :
> 0b1111110........ / 8  [starts 0x7E00  (32256)]
>1 kbits :
> 0b11111110....... / 7  [starts 0x7F00  (32512)]
>64 bytes :
> 0b111110111...... / 6  [starts 0x7DC0  (32192)]
> 0b111111110...... / 6  [starts 0x7F80  (32640)]
>32 bytes :
> 0b1111101101..... / 5  [starts 0x7DA0  (32160)]
>16 bytes :
> 0b11111011001.... / 4  [starts 0x7D90  (32144)]
> 0b11111111100.... / 4  [starts 0x7FC0  (32704)]


Here, it shows there is a 2kbit empty space (8 address lines) at 0x7E00 which will to go 0x7EFF (256 bytes)

Additionally, there is a space of 1kbits at 0x7F00, which will end at 0x7F7F (128 bytes)

There are additionalally 2 blocks of 64 bytes at 0x7DC0 and 0x7F80... etc

I have included two example ROM images to play with. (sadly, the above is one of them)


--------------------------------------

COMPILING:

gcc main.c -o blockfind


yup, thats it.


----------------------------------------

USE:

The intended use is as follows:

1) read the ROM your targeting to a file
2) run this program
3) if there is not a block available with the space you need for your projet (were not all using this for 7 segment decoders, are we?) select another ROM and goto(1);
4) use dd to write the image for your project over the blank area of the target ROM (this is why I gave you the start address in decimal)
5) write the modified image back to the ROM
6) install ROM in your project with the high address lines tied high or low according to the binary hint in the finder. (thats why I gave you the start address in binary, and constrained the binary to the number of address bits in your ROM)
7) play.

If you always use the smallest region your project requires, you could probably get a few projects out of a ROM!


Rue.






































