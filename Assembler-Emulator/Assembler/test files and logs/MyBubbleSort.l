DATA Segment:
Number of data items = 0000000A

	DEADBEEF
	5EEDBED5
	C0EDBABE
	5EAF00D5
	AB5C155A
	CA5CADED
	FEEDFACE
	C0C0F00D
	DECEA5ED
	50FABED5

CODE Segment:

			; outline of bubble sort program
0000 00100000 ldc 0x1000
0001 0000000B a2sp
0002 FFFFFF0A adj -1
0003 00000A00 ldc 10
0004 00000003 stl 0
0005 00004100 ldc array
0006 0000000D call sort
0007 00000012 halt
			;
			; Areg= return address
			; Breg= address of elements
			; SP+0= number of elements sort
			; int* sort(int* in,int count)
			;{
			;	int x;
			;	int y;
			;	int temp;
			;	for(x=0;x < count-1;x++)
			;	{
			;		for(y=1;y<count-x;y++)
			;		{
			;			if((*(in+y))>(*(in+y-1)))
			;			{
			;				temp=(*(in+y-1));
			;				(*(in+y-1))=(*(in+y));
			;				(*(in+y))=temp;
			;			}
			;		}
			;	}
			;	exit:return;
			;}
0008 FFFFFF0A sort: adj -1
0009 00000003 stl 0
			;save return address
000A 00000203 stl 2
			;save address of elements
000B FFFFFD0A adj -3
000C 00000000 ldc 0
000D 00000003 stl 0
			;save temp
000E 00000100 ldc 1
000F 00000103 stl 1
			;save y
0010 00000000 ldc 0
0011 00000203 stl 2
			;save x
0012 00000402 loop_out: ldl 4
			;load count
0013 00000202 ldl 2
			;load x
0014 00000007 sub
			;count - x
0015 00000100 ldc 1
0016 00000007 sub
			;count-1-x
0017 0000250F brz done
0018 00000100 ldc 1
0019 00000103 stl 1
			;y = 1
001A 00000402 loop_in: ldl 4
			;load count
001B 00000202 ldl 2
			;load x
001C 00000007 sub
			;count - x
001D 00000102 ldl 1
			;load y
001E 00000007 sub
			;count - x -y
001F 0000150F brz addx
0020 00000502 ldl 5
			;load address of elements
0021 00000004 ldnl 0
			;array[0]
0022 00000000 ldc 0
			;save array[0] to Breg
0023 00000104 ldnl 1
			;array[1]
0024 00000007 sub
			;array[0]-array[1]
0025 00000010 brlz swap
0026 00000911 br addy
0027 00000502 swap: ldl 5
			;load address of elements
0028 00000004 ldnl 0
			;array[0]
0029 00000003 stl 0
			;temp = array[0]
002A 00000502 ldl 5
002B 00000104 ldnl 1
			;array[1]
002C 00000502 ldl 5
002D 00000005 stnl 0
			;array[0] = array[1]
002E 00000002 ldl 0
			;load temp
002F 00000502 ldl 5
0030 00000105 stnl 1
			;array[1] = temp
			;increment
0031 00000100 addy: ldc 1
0032 00000102 ldl 1
			;load y
0033 00000006 add
			;y++
0034 00000103 stl 1
			;save y++
0035 FFFFE311 br loop_in
0036 00000100 addx: ldc 1
0037 00000202 ldl 2
			;load x
0038 00000006 add
			;x++
0039 00000203 stl 2
003A 00000100 ldc 1
003B 00000502 ldl 5
003C 00000006 add
			;array++
003D FFFFD311 br loop_out
003E 00000302 done: ldl 3
			;load return address
003F 0000050A adj 5
			;sp = array
0040 0000000E return
			;ok,
0041 array: data -559038737
0042 data 1592639189
0043 data -1058161986
0044 data 1588527317
0045 data -1420028582
0046 data -899895827
0047 data -17958194
0048 data -1061097459
0049 data -556882451
004A data 1358610133
