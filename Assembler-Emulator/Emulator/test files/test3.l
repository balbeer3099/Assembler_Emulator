DATA Segment:
Number of data items = 00000000

	00000000

CODE Segment:

			; test3.asm
			; Test SET
0000 val: set 75
0001 00004B00 ldc     val
0002 00004201 adc     val2
0003 val2: set 66
