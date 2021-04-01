DATA Segment:
Number of data items = 00000001

	00000000

CODE Segment:

			; test1.asm
0000 label:
0000 00000000 	ldc 0
0001 FFFFFB00 	ldc -5
0002 00000500 	ldc +5
0003 FFFFFF11 loop: br loop
0004 00000011 br next
0005 next:
0005 00000400     	ldc loop
0006 00000900 	ldc var1
0007 var1: data 0
