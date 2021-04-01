DATA Segment:
Number of data items = 00000000

	00000000

CODE Segment:

			; let's check comment works or not
0000 start:
0000 00000000 ldc 0
0001 00000201 adc 2
0002 00000201 adc 2
0003 FFFFFF01 adc -1
0004 00000201 adc 2
0005 var1: set 3
0006 00000301 adc var1
0007 end:
0007 00000012 halt
