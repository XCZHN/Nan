const unsigned char manage02[]={
/*ʱ��
Image size:48 X 48 X 16BPP=4608 bytes.
4 bytes=P(x,y)+P(x+1,y)
Data format: 4 bytes(2 pixels)=RRRRRGGG,GGGBBBBB,RRRRRGGG,GGGBBBBB
*/
0xff,0xff,0xff,0xff,0xf7,0xbe,0xbd,0xf7,0x9c,0xf3,0x94,0xb2,0x8c,0x71,0x8c,0x31,
0x84,0x31,0x84,0x31,0x83,0xd0,0x83,0xd0,0x7b,0xcf,0x7b,0xcf,0x73,0x8e,0x73,0x8e,
0x73,0x8e,0x6b,0x4d,0x6b,0x4d,0x63,0x0d,0x63,0x0c,0x5a,0xcc,0x5a,0xcb,0x5a,0xcb,
0x5a,0xca,0x52,0x8a,0x52,0x8a,0x4a,0x49,0x4a,0x49,0x4a,0x08,0x42,0x08,0x42,0x08,
0x39,0xc7,0x39,0xc7,0x39,0xc7,0x31,0xc6,0x31,0x86,0x31,0x86,0x42,0x48,0x5a,0xcb,
0x31,0xc7,0x29,0x45,0x21,0x45,0x29,0x45,0x42,0x08,0xa4,0xf2,0xf7,0xbe,0xff,0xff,
0xff,0xff,0xe7,0x3c,0xa5,0x34,0x94,0xb3,0x94,0x72,0x8c,0x71,0x8c,0x31,0x8c,0x30,
0x84,0x30,0x83,0xd0,0x83,0xcf,0x7b,0xcf,0x7b,0xcf,0x7b,0xce,0x73,0x8e,0x73,0x8e,
0x73,0x8d,0x6b,0x4d,0x6b,0x4d,0x63,0x4c,0x63,0x0c,0x5b,0x0b,0x5a,0xcb,0x5a,0xcb,
0x52,0x8a,0x52,0x8a,0x4a,0x4a,0x4a,0x49,0x4a,0x49,0x4a,0x08,0x42,0x08,0x41,0xc8,
0x39,0xc7,0x39,0xc7,0x39,0xc7,0x31,0x86,0x31,0x86,0x31,0x86,0x29,0x46,0x29,0x45,
0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x45,0x21,0x45,0x29,0x45,0x6b,0x4c,0xf7,0xbe,
0xf7,0xbe,0xad,0x75,0x94,0xb2,0x94,0xb1,0x94,0x71,0x8c,0x71,0x8c,0x70,0x8c,0x30,
0x84,0x30,0x84,0x30,0x84,0x2f,0x7b,0xcf,0x7b,0xcf,0x73,0x8e,0x73,0x8e,0x6b,0x8e,
0x6b,0x4d,0x6b,0x4d,0x6b,0x4d,0x63,0x0c,0x63,0x0c,0x5b,0x0b,0x5a,0xcb,0x52,0xca,
0x52,0x8a,0x52,0x8a,0x4a,0x49,0x4a,0x49,0x4a,0x49,0x42,0x08,0x42,0x08,0x42,0x08,
0x39,0xc7,0x39,0xc7,0x39,0xc7,0x31,0x86,0x31,0x86,0x31,0x86,0x31,0x46,0x29,0x45,
0x29,0x45,0x29,0x45,0x29,0x45,0x21,0x45,0x21,0x04,0x21,0x04,0x29,0x45,0x83,0xce,
0xce,0x79,0x94,0xf3,0x8c,0xb2,0x8c,0xb2,0x8c,0xb1,0x8c,0x71,0x84,0x71,0x84,0x31,
0x84,0x30,0x84,0x30,0x7c,0x30,0x7b,0xcf,0x7b,0xcf,0x73,0x8f,0x73,0x8e,0x6b,0x4d,
0x6b,0x4d,0x6b,0x4d,0x6b,0x0c,0x63,0x0c,0x63,0x0c,0x63,0x0c,0x63,0x0c,0x5a,0xcb,
0x5a,0xcb,0x52,0x8a,0x52,0x8a,0x4a,0x49,0x4a,0x49,0x42,0x08,0x42,0x08,0x3a,0x08,
0x39,0xc7,0x39,0xc7,0x31,0xc7,0x31,0x86,0x31,0x86,0x31,0x86,0x29,0x85,0x29,0x45,
0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x45,0x21,0x04,0x21,0x04,0x31,0x86,
0xb5,0xb6,0x8c,0x72,0x8c,0x71,0x8c,0x72,0x84,0x71,0x84,0x71,0x84,0x71,0x84,0x71,
0x84,0x70,0x84,0x30,0x84,0x30,0x7c,0x30,0x7b,0xd0,0x7b,0xcf,0x73,0x8f,0x73,0x8e,
0x73,0x4e,0x73,0x4e,0x6b,0x4d,0x63,0x0c,0x52,0x8a,0x4a,0x49,0x42,0x08,0x39,0xc7,
0x39,0xc7,0x39,0xc7,0x42,0x08,0x42,0x49,0x4a,0x49,0x4a,0x49,0x4a,0x49,0x42,0x08,
0x39,0xc7,0x39,0xc7,0x39,0xc7,0x31,0x86,0x31,0x86,0x31,0x86,0x29,0x45,0x29,0x45,
0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x45,0x21,0x05,0x21,0x05,0x21,0x04,0x29,0x45,
0x9c,0xf3,0x9c,0xb2,0x94,0x72,0x94,0x71,0x94,0x71,0x94,0x31,0x8c,0x31,0x8c,0x30,
0x84,0x30,0x84,0x30,0x7b,0xcf,0x7b,0xcf,0x7b,0xcf,0x7b,0x8e,0x7b,0x8e,0x7b,0x8e,
0x6b,0x0d,0x4a,0x49,0x31,0x86,0x21,0x05,0x21,0x04,0x21,0x04,0x29,0x04,0x29,0x45,
0x29,0x45,0x29,0x04,0x21,0x04,0x21,0x04,0x29,0x04,0x31,0x86,0x39,0xc7,0x42,0x08,
0x42,0x08,0x39,0xc7,0x39,0xc6,0x31,0x86,0x31,0x86,0x31,0x86,0x31,0x46,0x29,0x45,
0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x05,0x21,0x05,0x21,0x04,0x21,0x04,0x29,0x45,
0x9c,0xb2,0x9c,0xb3,0x94,0x72,0x94,0x72,0x94,0x71,0x8c,0x71,0x8c,0x70,0x84,0x70,
0x84,0x30,0x84,0x30,0x7b,0xd0,0x7b,0xcf,0x7b,0xcf,0x83,0xcf,0x6b,0x4d,0x42,0x07,
0x21,0x04,0x21,0x04,0x29,0x45,0x39,0xc7,0x39,0xc7,0x39,0xc7,0x31,0x86,0x31,0x86,
0x31,0x86,0x31,0x86,0x39,0xc7,0x39,0xc7,0x39,0xc7,0x29,0x45,0x21,0x04,0x21,0x04,
0x31,0x86,0x42,0x08,0x39,0xc7,0x31,0x86,0x31,0x86,0x31,0x86,0x31,0x86,0x29,0x45,
0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x45,0x21,0x05,0x21,0x04,0x21,0x04,0x21,0x05,
0x94,0xf2,0x94,0xb2,0x94,0xb2,0x94,0xb1,0x8c,0x71,0x8c,0x71,0x84,0x71,0x84,0x70,
0x84,0x30,0x84,0x2f,0x83,0xcf,0x83,0xcf,0x7b,0xcf,0x4a,0x49,0x21,0x04,0x21,0x04,
0x31,0x87,0x39,0xc7,0x31,0x86,0x21,0x05,0x21,0x04,0x29,0x45,0x31,0x86,0x39,0xc6,
0x39,0xc6,0x31,0x86,0x29,0x45,0x21,0x04,0x29,0x05,0x31,0x86,0x39,0xc7,0x31,0x86,
0x21,0x04,0x29,0x05,0x31,0x86,0x39,0xc7,0x31,0x86,0x31,0x86,0x29,0x45,0x29,0x45,
0x29,0x45,0x29,0x45,0x29,0x45,0x21,0x45,0x29,0x04,0x29,0x04,0x21,0x04,0x29,0x05,
0x94,0xf2,0x94,0xf2,0x94,0xb1,0x94,0xb1,0x94,0x71,0x8c,0x70,0x8c,0x30,0x84,0x30,
0x84,0x30,0x84,0x30,0x84,0x30,0x6b,0x8e,0x31,0x86,0x21,0x04,0x31,0x86,0x39,0xc7,
0x29,0x45,0x21,0x04,0x42,0x07,0x5a,0xc9,0x7b,0x8c,0x8c,0x2d,0x94,0x6e,0x94,0x6e,
0x94,0x6e,0x94,0x6e,0x84,0x2d,0x73,0x8c,0x5a,0xc9,0x39,0xc6,0x21,0x04,0x29,0x45,
0x39,0xc7,0x31,0x86,0x21,0x04,0x29,0x45,0x39,0xc7,0x31,0x86,0x29,0x45,0x29,0x45,
0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x04,0x21,0x04,0x29,0x45,
0x9c,0xb2,0x9c,0xb2,0x94,0xb2,0x94,0x71,0x94,0x71,0x8c,0x71,0x8c,0x70,0x8c,0x30,
0x84,0x31,0x8c,0x71,0x6b,0x4d,0x29,0x45,0x21,0x04,0x39,0xc7,0x31,0x86,0x21,0x04,
0x52,0x89,0x94,0xb0,0xa4,0xf0,0x9c,0xef,0xad,0x30,0xad,0x31,0xb5,0x71,0xb5,0xb2,
0xb5,0xb2,0xb5,0x71,0xad,0x31,0xa5,0x30,0xa4,0xef,0x94,0x6e,0x73,0x8c,0x42,0x07,
0x21,0x04,0x31,0x86,0x39,0xc7,0x29,0x45,0x29,0x45,0x39,0x86,0x31,0x86,0x29,0x45,
0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x04,0x21,0x04,0x21,0x04,0x21,0x45,
0x94,0xb2,0x94,0xb3,0x8c,0xb2,0x94,0x72,0x8c,0x72,0x8c,0x71,0x8c,0x71,0x8c,0x71,
0x8c,0x71,0x6b,0x4d,0x21,0x05,0x21,0x04,0x39,0xc7,0x29,0x45,0x39,0x86,0x8c,0x6f,
0xbd,0xb3,0xbd,0xb4,0xb5,0x72,0xbd,0xb2,0xc5,0xf3,0xce,0x34,0xce,0x34,0xcd,0x71,
0xcd,0x31,0xce,0x34,0xce,0x34,0xc5,0xf3,0xbd,0xb2,0xb5,0x71,0xa4,0xf0,0x9c,0xaf,
0x6b,0x4b,0x29,0x45,0x29,0x45,0x39,0xc7,0x29,0x45,0x29,0x45,0x31,0x86,0x31,0x86,
0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x05,0x21,0x04,0x21,0x04,0x21,0x04,
0x94,0xf2,0x94,0xf2,0x94,0xb1,0x94,0xb1,0x8c,0xb1,0x8c,0xb1,0x8c,0x71,0x8c,0xb1,
0x73,0x8e,0x29,0x85,0x21,0x04,0x39,0xc7,0x21,0x04,0x4a,0x49,0xad,0x32,0xbd,0xf4,
0xce,0x35,0xd6,0x76,0xd6,0x74,0xde,0x75,0xde,0xb6,0xe6,0xf6,0xde,0x74,0xd4,0xf0,
0xd4,0xaf,0xde,0x34,0xde,0xf6,0xde,0xb6,0xde,0x75,0xd6,0x75,0xc5,0xf3,0xb5,0x71,
0xa4,0xf0,0x83,0xcd,0x39,0xc7,0x21,0x05,0x39,0xc7,0x29,0x45,0x29,0x45,0x31,0x86,
0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x45,0x21,0x04,0x21,0x04,0x21,0x04,0x21,0x04,
0x9c,0xf2,0x9c,0xb2,0x94,0xb1,0x94,0xb1,0x8c,0xb1,0x84,0x71,0x8c,0xb2,0x7b,0xd0,
0x39,0xc7,0x21,0x04,0x39,0xc7,0x29,0x45,0x52,0x89,0xb5,0x73,0xc5,0xf4,0xd6,0x76,
0xd6,0x76,0xb5,0x72,0xb5,0xb2,0xde,0x75,0xee,0xf7,0xee,0xf7,0xee,0xf7,0xef,0x37,
0xee,0xf7,0xee,0xf7,0xe6,0xf7,0xef,0x37,0xde,0xb5,0xbd,0xb2,0xbd,0xb2,0xce,0x34,
0xbd,0xb2,0xad,0x30,0x8c,0x2e,0x42,0x07,0x21,0x04,0x39,0xc7,0x29,0x45,0x29,0x45,
0x31,0x86,0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x05,0x21,0x04,0x21,0x04,0x29,0x45,
0x94,0xf2,0x94,0xf2,0x94,0xb1,0x8c,0xb1,0x8c,0xb1,0x94,0xb1,0x8c,0xb1,0x52,0xcb,
0x21,0x04,0x31,0x86,0x29,0x45,0x4a,0x49,0xb5,0x73,0xc5,0xf4,0xd6,0x76,0xe6,0xf7,
0xde,0xf7,0xc5,0xf4,0xbd,0xb2,0xd6,0x75,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,
0xef,0x37,0xef,0x37,0xef,0x37,0xf7,0x37,0xde,0xb6,0x94,0xaf,0xad,0x31,0xde,0xb5,
0xd6,0x75,0xbd,0xb3,0xad,0x31,0x84,0x2d,0x42,0x07,0x21,0x05,0x39,0xc7,0x29,0x45,
0x29,0x45,0x29,0x86,0x21,0x45,0x21,0x45,0x21,0x45,0x21,0x04,0x21,0x04,0x29,0x45,
0x9c,0xf2,0x94,0xf2,0x8c,0xb1,0x8c,0xb1,0x8c,0x71,0x94,0xb2,0x73,0x8e,0x29,0x45,
0x29,0x45,0x31,0x86,0x39,0xc7,0xa5,0x32,0xc5,0xf4,0xd6,0x76,0xe6,0xf7,0xef,0x38,
0xef,0x39,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,
0xef,0x37,0xef,0x37,0xef,0x37,0xee,0xf7,0xef,0x37,0xef,0x37,0xe6,0xf6,0xe6,0xf6,
0xde,0xb6,0xd6,0x75,0xbd,0xb3,0xad,0x30,0x7b,0x8c,0x31,0x86,0x29,0x45,0x31,0x86,
0x29,0x05,0x31,0x86,0x29,0x45,0x21,0x44,0x21,0x44,0x21,0x04,0x21,0x04,0x21,0x45,
0x9c,0xf3,0x9c,0xb3,0x94,0xb2,0x94,0x72,0x94,0xb2,0x8c,0x71,0x4a,0x4a,0x21,0x04,
0x31,0x86,0x29,0x85,0x83,0xce,0xc5,0xf5,0xd6,0x76,0xe6,0xf8,0xef,0x38,0xef,0x39,
0xef,0x39,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xf7,0x37,
0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xee,0xf7,
0xe6,0xf6,0xde,0xb6,0xd6,0x75,0xbd,0xb2,0xa4,0xef,0x62,0xca,0x21,0x05,0x31,0x86,
0x29,0x45,0x29,0x45,0x29,0x45,0x21,0x04,0x21,0x45,0x21,0x04,0x21,0x04,0x29,0x44,
0x9c,0xb3,0x9c,0xb3,0x94,0x72,0x94,0x72,0x9c,0xb3,0x7b,0xcf,0x31,0x86,0x29,0x45,
0x29,0x86,0x4a,0x49,0xbd,0xb3,0xce,0x35,0xd6,0x76,0xde,0xb7,0xef,0x38,0xef,0x39,
0xef,0x39,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xee,0xf7,0xd6,0x75,
0xe6,0xf6,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xee,0xf7,
0xee,0xf7,0xee,0xf7,0xde,0xb6,0xce,0x34,0xb5,0x71,0x8c,0x2d,0x42,0x07,0x21,0x05,
0x31,0x86,0x21,0x04,0x29,0x86,0x21,0x45,0x21,0x45,0x21,0x04,0x21,0x04,0x29,0x45,
0x9c,0xf2,0x9c,0xf3,0x94,0xb2,0x94,0xb2,0x94,0xb2,0x5a,0xcb,0x21,0x04,0x29,0x45,
0x31,0x86,0x84,0x2e,0xce,0x35,0xd6,0x76,0xbd,0xb4,0xc5,0xf5,0xe6,0xf8,0xf7,0x7a,
0xef,0x39,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xe6,0xf6,0x4a,0x48,
0xa4,0xf0,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,
0xee,0xf7,0xbd,0xb3,0x9c,0xb0,0xb5,0x72,0xc5,0xf3,0xa4,0xf0,0x63,0x0a,0x29,0x45,
0x31,0x86,0x29,0x45,0x29,0x45,0x29,0x45,0x21,0x04,0x21,0x04,0x21,0x04,0x21,0x04,
0x94,0xf2,0x94,0xf2,0x8c,0xb2,0x94,0xf3,0x8c,0x71,0x42,0x08,0x21,0x04,0x29,0x45,
0x4a,0x08,0xad,0x72,0xce,0x35,0xde,0xb7,0xef,0x39,0xf7,0x79,0xf7,0x79,0xef,0x79,
0xef,0x39,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xe6,0xf6,0x4a,0x48,
0xa4,0xf0,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,
0xef,0x37,0xce,0x34,0xbd,0xb3,0xd6,0x34,0xce,0x34,0xb5,0x72,0x83,0xcd,0x39,0xc6,
0x29,0x45,0x31,0x86,0x29,0x45,0x29,0x45,0x21,0x04,0x21,0x04,0x21,0x04,0x21,0x04,
0x94,0xf2,0x94,0xb2,0x8c,0xb1,0x9c,0xb2,0x7b,0x8f,0x31,0x86,0x21,0x04,0x31,0x86,
0x63,0x0b,0xc5,0xf5,0xd6,0x76,0xe6,0xf8,0xef,0x39,0xef,0x39,0xef,0x79,0xf7,0x79,
0xef,0x39,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xe6,0xf7,0x4a,0x48,
0xa4,0xf0,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,
0xef,0x37,0xef,0x37,0xef,0x37,0xe6,0xf6,0xd6,0x75,0xbd,0xb2,0x9c,0xaf,0x52,0x89,
0x21,0x04,0x31,0x86,0x21,0x04,0x29,0x45,0x21,0x04,0x21,0x44,0x21,0x04,0x21,0x45,
0x94,0xf2,0x94,0xf2,0x94,0xb1,0x9c,0xf3,0x6b,0x4d,0x29,0x45,0x21,0x05,0x39,0x86,
0x84,0x2e,0xce,0x35,0xd6,0xb6,0xe6,0xf8,0xef,0x39,0xef,0x79,0xf7,0x79,0xf7,0x79,
0xef,0x39,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xe6,0xf7,0x4a,0x48,
0xa4,0xf0,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,
0xef,0x37,0xef,0x37,0xef,0x37,0xe6,0xf7,0xde,0xb5,0xc5,0xf3,0xa4,0xf0,0x6b,0x0a,
0x21,0x05,0x31,0x86,0x29,0x45,0x29,0x45,0x21,0x44,0x21,0x44,0x21,0x04,0x21,0x45,
0x9c,0xb2,0x9c,0xb2,0x94,0x72,0x9c,0xb2,0x5b,0x0b,0x29,0x45,0x29,0x45,0x39,0xc7,
0x9c,0xb0,0xce,0x35,0xde,0xf7,0xef,0x39,0xef,0x79,0xef,0x79,0xef,0x79,0xef,0x79,
0xef,0x79,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xe6,0xf7,0x4a,0x48,
0xa4,0xf0,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,
0xef,0x37,0xef,0x37,0xef,0x37,0xee,0xf7,0xde,0xb6,0xce,0x34,0xad,0x31,0x73,0x8b,
0x29,0x45,0x31,0x86,0x29,0x45,0x29,0x45,0x29,0x45,0x21,0x04,0x21,0x04,0x29,0x45,
0x9c,0xf3,0x94,0xf2,0x94,0xb2,0x94,0xb2,0x5a,0xca,0x21,0x05,0x29,0x45,0x42,0x08,
0xa5,0x32,0xce,0x35,0xdd,0xf4,0xe6,0xb6,0xef,0x39,0xf7,0x7a,0xff,0xba,0xff,0xba,
0xff,0xba,0xf7,0x79,0xf7,0x78,0xf7,0x78,0xf7,0x78,0xf7,0x78,0xe6,0xf7,0x4a,0x48,
0xad,0x31,0xf7,0x78,0xf7,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,
0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xe6,0xf6,0xce,0x34,0xb5,0x72,0x7b,0xcc,
0x31,0x86,0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x45,0x21,0x04,0x21,0x04,0x21,0x04,
0x9c,0xf3,0x94,0xb3,0x94,0xb2,0x94,0xf2,0x52,0x8a,0x21,0x04,0x29,0x45,0x4a,0x48,
0xad,0x73,0xcd,0xf4,0xd5,0x32,0xdd,0x73,0xee,0x76,0xde,0xf8,0xb5,0x73,0xbd,0xb3,
0xbd,0xb3,0xb5,0xb3,0xb5,0x72,0xb5,0x72,0xb5,0x72,0xbd,0xb2,0x6b,0x4b,0x29,0x04,
0x83,0xcd,0xbd,0xb2,0xde,0xb6,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,
0xef,0x37,0xef,0x37,0xef,0x37,0xe6,0x75,0xd4,0xf0,0xc4,0xaf,0xb5,0x71,0x84,0x2d,
0x39,0xc6,0x29,0x45,0x29,0x86,0x29,0x45,0x29,0x45,0x21,0x04,0x21,0x04,0x29,0x05,
0x9c,0xf3,0x94,0xb2,0x94,0xb2,0x94,0xb2,0x52,0x8a,0x21,0x04,0x29,0x45,0x4a,0x48,
0xad,0x72,0xcd,0xf5,0xd5,0x73,0xe5,0xf5,0xef,0x39,0xce,0x36,0x7b,0xce,0x84,0x2e,
0x8c,0x2f,0x8c,0x2e,0x84,0x2d,0x84,0x2d,0x84,0x2d,0x8c,0x2e,0x84,0x2d,0x21,0x03,
0x31,0x85,0x8c,0x2d,0xd6,0x75,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,
0xef,0x37,0xef,0x37,0xef,0x38,0xe6,0xb5,0xd4,0xf0,0xc4,0xaf,0xb5,0x72,0x84,0x2d,
0x39,0xc6,0x29,0x45,0x31,0x86,0x29,0x45,0x29,0x45,0x21,0x04,0x21,0x04,0x21,0x45,
0x9c,0xf3,0x94,0xb3,0x94,0xb2,0x94,0xb2,0x5a,0x8a,0x21,0x44,0x29,0x45,0x42,0x08,
0xa5,0x32,0xce,0x35,0xdd,0xf4,0xe6,0xb6,0xef,0x39,0xf7,0x7a,0xff,0xbb,0xff,0xbb,
0xff,0xbb,0xff,0xba,0xf7,0x78,0xf7,0x78,0xf7,0x78,0xff,0x78,0xf7,0x78,0x8c,0x6e,
0x73,0x8c,0xa4,0xf0,0xee,0xf7,0xf7,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,
0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xe6,0xf6,0xce,0x74,0xb5,0x71,0x7b,0xcc,
0x31,0x86,0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x45,0x21,0x04,0x21,0x04,0x21,0x04,
0x9c,0xf3,0x94,0xb3,0x94,0xb2,0x94,0xb2,0x5a,0xcb,0x29,0x45,0x29,0x45,0x3a,0x07,
0x9c,0xb0,0xce,0x76,0xde,0xf7,0xef,0x39,0xef,0x79,0xef,0x79,0xef,0x79,0xef,0x79,
0xef,0x79,0xef,0x79,0xef,0x38,0xee,0xf7,0xef,0x37,0xee,0xf7,0xee,0xf7,0xf7,0x78,
0xff,0x78,0x94,0x6e,0x94,0x6e,0xe6,0xb6,0xf7,0x37,0xef,0x37,0xef,0x37,0xef,0x37,
0xef,0x37,0xef,0x37,0xef,0x37,0xee,0xf7,0xde,0xb6,0xce,0x34,0xb5,0x71,0x73,0x8c,
0x31,0x86,0x31,0x86,0x29,0x45,0x29,0x45,0x29,0x45,0x21,0x04,0x21,0x04,0x21,0x04,
0x9c,0xf2,0x94,0xb2,0x8c,0x72,0x9c,0xf3,0x6b,0x4d,0x29,0x45,0x21,0x45,0x39,0xc6,
0x8c,0x2e,0xce,0x35,0xd6,0xb6,0xe6,0xf8,0xef,0x39,0xef,0x79,0xf7,0x79,0xf7,0x79,
0xf7,0x79,0xf7,0x79,0xef,0x39,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,
0xef,0x37,0xff,0x78,0x9c,0xaf,0x8c,0x2e,0xde,0xb6,0xf7,0x78,0xef,0x37,0xef,0x37,
0xef,0x37,0xef,0x37,0xef,0x37,0xe6,0xf6,0xde,0xb5,0xc5,0xf3,0xad,0x30,0x6b,0x0a,
0x29,0x45,0x31,0x86,0x29,0x45,0x29,0x45,0x29,0x45,0x21,0x44,0x21,0x04,0x29,0x45,
0x9c,0xf2,0x94,0xb2,0x94,0x71,0x9c,0xf3,0x73,0x8f,0x31,0x86,0x21,0x04,0x31,0x86,
0x6b,0x4c,0xc6,0x35,0xd6,0x76,0xe6,0xf8,0xef,0x39,0xef,0x39,0xef,0x79,0xf7,0x79,
0xf7,0x79,0xf7,0x79,0xf7,0x79,0xef,0x38,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,
0xef,0x37,0xef,0x37,0xff,0x78,0xa4,0xf0,0x8c,0x2d,0xde,0xb5,0xf7,0x78,0xef,0x37,
0xef,0x37,0xef,0x37,0xef,0x37,0xe6,0xf6,0xd6,0x75,0xbd,0xb2,0x9c,0xaf,0x5a,0x89,
0x21,0x04,0x31,0x87,0x21,0x04,0x29,0x46,0x21,0x05,0x21,0x05,0x21,0x04,0x21,0x04,
0x9c,0xf2,0x94,0xb2,0x8c,0x71,0x94,0xb2,0x84,0x30,0x42,0x08,0x21,0x04,0x29,0x46,
0x4a,0x48,0xb5,0xb3,0xce,0x76,0xde,0xb7,0xbd,0xf4,0xce,0x35,0xef,0x39,0xf7,0x7a,
0xf7,0x79,0xf7,0x79,0xf7,0x79,0xef,0x39,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,
0xef,0x37,0xef,0x37,0xef,0x37,0xff,0xb9,0xa4,0xf0,0x84,0x2d,0xd6,0x75,0xf7,0x78,
0xef,0x37,0xd6,0x75,0xce,0x34,0xde,0xb6,0xce,0x34,0xb5,0x72,0x84,0x2d,0x41,0xc7,
0x21,0x45,0x31,0x86,0x29,0x45,0x29,0x45,0x21,0x04,0x21,0x04,0x21,0x04,0x21,0x04,
0x9c,0xf2,0x94,0xb2,0x94,0x71,0x94,0xb1,0x94,0xb2,0x52,0xca,0x21,0x04,0x29,0x45,
0x31,0x86,0x8c,0x6f,0xce,0x35,0xd6,0x76,0xce,0x36,0xd6,0xb7,0xef,0x39,0xf7,0x79,
0xef,0x79,0xf7,0x79,0xf7,0x79,0xf7,0x79,0xef,0x38,0xef,0x37,0xef,0x37,0xef,0x37,
0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xff,0xb9,0xad,0x31,0x83,0xcd,0xd6,0x34,
0xf7,0x78,0xce,0x34,0xb5,0x71,0xc5,0xf3,0xc5,0xf3,0xad,0x31,0x6b,0x4b,0x29,0x45,
0x31,0x86,0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x44,0x21,0x44,0x21,0x04,0x29,0x45,
0x9c,0xf3,0x94,0xb3,0x94,0x72,0x94,0x71,0x9c,0xf2,0x73,0x8e,0x29,0x45,0x29,0x45,
0x31,0x86,0x5a,0x89,0xc5,0xf4,0xc6,0x35,0xbd,0xf4,0xce,0x76,0xef,0x39,0xf7,0x79,
0xef,0x79,0xef,0x79,0xef,0x79,0xef,0x79,0xef,0x79,0xef,0x37,0xef,0x37,0xef,0x37,
0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xff,0xb9,0xb5,0x72,0x83,0xcd,
0xc5,0xf3,0xf7,0x37,0xe6,0xf6,0xd6,0x34,0xb5,0x72,0x94,0x6e,0x4a,0x48,0x21,0x04,
0x39,0xc7,0x21,0x04,0x31,0x86,0x29,0x45,0x21,0x04,0x21,0x05,0x21,0x04,0x21,0x05,
0x9c,0xf3,0x9c,0xb3,0x94,0x72,0x94,0x71,0x94,0xb2,0x8c,0x71,0x4a,0x49,0x21,0x04,
0x31,0x86,0x31,0x86,0x8c,0x6f,0xc6,0x35,0xd6,0x76,0xe6,0xf8,0xef,0x38,0xef,0x39,
0xef,0x79,0xf7,0x79,0xf7,0x79,0xf7,0x79,0xef,0x79,0xef,0x39,0xef,0x37,0xef,0x37,
0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xff,0xb8,0xb5,0xb2,
0x8c,0x2e,0xc5,0xf3,0xde,0xb6,0xbd,0xb2,0xa4,0xf0,0x6b,0x0a,0x29,0x45,0x31,0x86,
0x29,0x45,0x29,0x45,0x31,0x45,0x21,0x04,0x21,0x45,0x21,0x05,0x21,0x04,0x29,0x45,
0x94,0xb2,0x9c,0xf3,0x94,0xb2,0x94,0xb1,0x8c,0xb1,0x94,0xb2,0x6b,0x4d,0x29,0x45,
0x29,0x45,0x29,0x46,0x42,0x07,0xb5,0x73,0xc5,0xf5,0xd6,0x76,0xe6,0xf8,0xef,0x39,
0xe6,0xf8,0xc6,0x35,0xc6,0x35,0xe7,0x38,0xf7,0x79,0xf7,0x79,0xef,0x39,0xef,0x37,
0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xf7,0x37,0xef,0x37,0xef,0x37,
0xde,0xb5,0xce,0x34,0xc5,0xf3,0xad,0x31,0x83,0xcd,0x39,0xc6,0x29,0x45,0x39,0xc6,
0x21,0x04,0x31,0x86,0x29,0x45,0x21,0x05,0x21,0x04,0x21,0x04,0x21,0x04,0x29,0x45,
0x94,0xb2,0x94,0xb3,0x8c,0x72,0x8c,0x72,0x8c,0x72,0x94,0x72,0x8c,0x72,0x4a,0x4a,
0x21,0x04,0x31,0x86,0x29,0x45,0x5a,0xca,0xbd,0xb4,0xc5,0xf5,0xd6,0x76,0xe6,0xf8,
0xe6,0xf8,0xc5,0xf5,0xc5,0xf5,0xe6,0xf8,0xf7,0x79,0xef,0x79,0xef,0x79,0xef,0x39,
0xef,0x37,0xef,0x37,0xef,0x37,0xef,0x37,0xd6,0x75,0xb5,0x72,0xce,0x34,0xde,0xb6,
0xde,0xb5,0xc5,0xf3,0xad,0x31,0x94,0x6e,0x4a,0x48,0x21,0x04,0x39,0xc7,0x29,0x45,
0x29,0x45,0x29,0x46,0x21,0x05,0x21,0x45,0x21,0x04,0x21,0x04,0x21,0x04,0x29,0x45,
0x94,0xb2,0x94,0xb2,0x8c,0x72,0x8c,0x72,0x8c,0x72,0x8c,0x72,0x94,0xb2,0x7b,0xcf,
0x31,0x86,0x21,0x04,0x31,0x87,0x21,0x04,0x6b,0x4c,0xbd,0xb4,0xc5,0xf4,0xd6,0x76,
0xde,0xb7,0xbd,0xb4,0xc5,0xf5,0xef,0x38,0xef,0x39,0xef,0x39,0xef,0x79,0xf7,0x7a,
0xef,0x39,0xef,0x37,0xee,0xf7,0xef,0x37,0xbd,0xb2,0x8c,0x2e,0xad,0x31,0xd6,0x34,
0xc5,0xf3,0xad,0x31,0x94,0x6e,0x52,0x48,0x21,0x04,0x31,0x87,0x29,0x45,0x29,0x45,
0x31,0x86,0x29,0x45,0x21,0x45,0x21,0x45,0x21,0x44,0x21,0x04,0x21,0x04,0x29,0x45,
0x94,0xb2,0x94,0xf3,0x94,0xb2,0x8c,0xb1,0x8c,0x71,0x8c,0x71,0x8c,0x31,0x8c,0x71,
0x6b,0x4d,0x21,0x04,0x29,0x45,0x39,0xc7,0x21,0x04,0x63,0x0b,0xb5,0xb3,0xc5,0xf4,
0xce,0x35,0xd6,0x76,0xde,0xb7,0xe6,0xf8,0xe6,0xf8,0xef,0x38,0xef,0x39,0xee,0xf7,
0xef,0x39,0xef,0x38,0xe6,0xf7,0xde,0xb6,0xde,0xb5,0xd6,0x74,0xc5,0xf3,0xb5,0xb2,
0xa4,0xf0,0x8c,0x6e,0x4a,0x48,0x21,0x04,0x31,0x87,0x31,0x86,0x21,0x45,0x31,0x86,
0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x45,0x21,0x05,0x21,0x05,0x21,0x04,0x29,0x45,
0x94,0xb2,0x9c,0xf2,0x94,0xb1,0x94,0xb1,0x8c,0x71,0x8c,0x71,0x8c,0x71,0x8c,0x71,
0x8c,0x71,0x63,0x0c,0x21,0x04,0x29,0x45,0x39,0xc7,0x21,0x04,0x4a,0x48,0xa4,0xf1,
0xbd,0xb4,0xc5,0xf5,0xce,0x35,0xd6,0x76,0xde,0xb7,0xde,0xf7,0xde,0x76,0xd4,0x2e,
0xd4,0xb0,0xde,0xb7,0xde,0xf8,0xd6,0x76,0xc5,0xf3,0xb5,0xb2,0xad,0x30,0x9c,0xef,
0x7b,0x8c,0x39,0xc6,0x21,0x04,0x39,0xc7,0x31,0x86,0x21,0x04,0x31,0x86,0x31,0x86,
0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x05,0x29,0x05,0x29,0x04,0x21,0x04,0x29,0x45,
0x94,0xb2,0x9c,0xb2,0x94,0x71,0x94,0x71,0x94,0x71,0x8c,0x71,0x8c,0x71,0x84,0x70,
0x84,0x71,0x8c,0x71,0x62,0xcc,0x21,0x04,0x29,0x45,0x39,0xc7,0x29,0x45,0x29,0x45,
0x6b,0x4c,0xa5,0x32,0xbd,0xb3,0xbd,0xf4,0xc5,0xf5,0xce,0x35,0xce,0x35,0xcd,0x72,
0xcd,0xf4,0xce,0x35,0xc6,0x35,0xc5,0xf5,0xbd,0xf4,0xad,0x72,0x8c,0x2e,0x52,0x89,
0x21,0x04,0x29,0x45,0x39,0xc7,0x29,0x45,0x21,0x04,0x31,0x86,0x31,0x86,0x29,0x45,
0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x05,0x29,0x04,0x21,0x04,0x29,0x45,
0x94,0xb2,0x94,0xb2,0x94,0x71,0x94,0x71,0x8c,0x71,0x8c,0x71,0x8c,0x70,0x8c,0x30,
0x84,0x30,0x84,0x30,0x8c,0x31,0x6b,0x0d,0x29,0x45,0x21,0x04,0x39,0xc7,0x39,0xc7,
0x21,0x04,0x29,0x45,0x5a,0xca,0x8c,0x2f,0xa4,0xf1,0xb5,0x72,0xb5,0xb3,0xbd,0xf4,
0xbd,0xb4,0xb5,0xb3,0xb5,0x72,0xa4,0xf1,0x84,0x2e,0x5a,0x8a,0x29,0x45,0x21,0x04,
0x39,0xc7,0x39,0xc6,0x21,0x04,0x29,0x45,0x39,0xc6,0x31,0x86,0x29,0x45,0x29,0x45,
0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x45,0x21,0x44,0x21,0x04,0x21,0x04,0x29,0x45,
0x94,0xb2,0x9c,0xb3,0x94,0x72,0x94,0x72,0x8c,0x72,0x8c,0x71,0x8c,0x71,0x84,0x31,
0x84,0x30,0x83,0xd0,0x83,0xd0,0x84,0x30,0x73,0x8e,0x39,0xc7,0x21,0x04,0x29,0x45,
0x39,0xc7,0x39,0xc7,0x29,0x45,0x21,0x04,0x29,0x45,0x39,0xc7,0x4a,0x48,0x52,0x89,
0x52,0x89,0x4a,0x48,0x39,0xc7,0x29,0x45,0x21,0x04,0x29,0x45,0x39,0xc7,0x39,0xc7,
0x29,0x45,0x21,0x04,0x31,0x86,0x39,0xc7,0x31,0x86,0x31,0x86,0x31,0x46,0x29,0x45,
0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x04,0x21,0x04,0x21,0x04,0x21,0x04,0x29,0x45,
0x94,0xb2,0x9c,0xb3,0x94,0x72,0x94,0x72,0x8c,0x71,0x8c,0x71,0x8c,0x71,0x84,0x71,
0x84,0x30,0x7c,0x30,0x7c,0x30,0x7b,0xcf,0x7b,0xcf,0x7b,0xcf,0x5a,0xcb,0x31,0x86,
0x21,0x04,0x29,0x45,0x31,0x86,0x39,0xc7,0x39,0xc7,0x31,0x86,0x29,0x46,0x29,0x45,
0x29,0x45,0x29,0x46,0x31,0x86,0x39,0xc7,0x39,0xc7,0x31,0x86,0x29,0x45,0x21,0x04,
0x29,0x45,0x39,0xc7,0x39,0xc7,0x31,0x86,0x31,0x86,0x31,0x86,0x29,0x85,0x29,0x45,
0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x45,0x21,0x05,0x21,0x04,0x21,0x04,0x31,0xc6,
0x9c,0xf2,0x9c,0xf3,0x94,0xb2,0x94,0xb2,0x8c,0x71,0x8c,0x71,0x8c,0x31,0x84,0x30,
0x84,0x30,0x84,0x30,0x84,0x2f,0x7b,0xcf,0x7b,0xcf,0x7b,0xce,0x7b,0xce,0x73,0x8e,
0x5a,0xcb,0x39,0xc8,0x29,0x45,0x21,0x04,0x21,0x04,0x29,0x45,0x29,0x45,0x29,0x46,
0x29,0x45,0x29,0x45,0x29,0x45,0x21,0x04,0x21,0x04,0x29,0x45,0x31,0x86,0x42,0x08,
0x42,0x08,0x39,0xc7,0x39,0xc7,0x31,0x86,0x31,0x86,0x31,0x86,0x29,0x46,0x29,0x45,
0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x05,0x21,0x04,0x21,0x04,0x31,0x86,
0x9c,0xf3,0x94,0xb2,0x94,0xb1,0x94,0x71,0x94,0x71,0x8c,0x71,0x8c,0x31,0x84,0x31,
0x84,0x31,0x83,0xd0,0x7b,0xd0,0x7b,0xcf,0x7b,0xcf,0x73,0xce,0x73,0x8e,0x73,0x8e,
0x73,0x8e,0x73,0x8e,0x6b,0x4d,0x5a,0xca,0x4a,0x49,0x3a,0x07,0x39,0xc7,0x31,0x86,
0x31,0x86,0x31,0x86,0x39,0xc7,0x42,0x08,0x42,0x09,0x4a,0x49,0x4a,0x49,0x42,0x08,
0x39,0xc7,0x39,0xc7,0x39,0xc7,0x31,0x86,0x31,0x86,0x31,0x86,0x29,0x86,0x29,0x45,
0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x44,0x29,0x44,0x21,0x04,0x21,0x04,0x31,0x86,
0xa5,0x74,0x8c,0xb2,0x8c,0xb2,0x8c,0xb1,0x8c,0x71,0x84,0x71,0x84,0x31,0x84,0x31,
0x84,0x30,0x84,0x30,0x83,0xcf,0x7b,0xcf,0x7b,0xcf,0x73,0x8f,0x73,0x8e,0x73,0x8e,
0x6b,0x4d,0x6b,0x4d,0x6b,0x4d,0x63,0x4d,0x63,0x0d,0x63,0x0d,0x62,0xcc,0x5a,0xcc,
0x5a,0xcb,0x52,0xcb,0x52,0x8a,0x4a,0x4a,0x4a,0x49,0x42,0x08,0x42,0x08,0x3a,0x08,
0x39,0xc7,0x39,0xc7,0x31,0xc7,0x31,0x86,0x31,0x86,0x31,0x86,0x29,0x46,0x29,0x45,
0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x44,0x21,0x04,0x21,0x04,0x4a,0x08,
0xd6,0xba,0x94,0xb2,0x94,0xb2,0x8c,0x71,0x8c,0x71,0x8c,0x71,0x84,0x71,0x84,0x30,
0x84,0x30,0x84,0x30,0x84,0x30,0x84,0x2f,0x7b,0xcf,0x73,0xce,0x73,0x8e,0x73,0x8e,
0x73,0x8e,0x6b,0x4d,0x6b,0x4d,0x63,0x4c,0x63,0x0c,0x5a,0xcb,0x5a,0xcb,0x5a,0xca,
0x52,0x8a,0x52,0x8a,0x4a,0x49,0x4a,0x49,0x4a,0x49,0x42,0x48,0x42,0x08,0x42,0x08,
0x39,0xc7,0x39,0xc7,0x39,0x87,0x31,0x86,0x31,0x86,0x31,0x86,0x29,0x45,0x29,0x45,
0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x05,0x21,0x04,0x29,0x45,0xa4,0xf3,
0xff,0xff,0xce,0x79,0x94,0xb2,0x94,0xb2,0x94,0x72,0x8c,0x71,0x8c,0x31,0x8c,0x30,
0x84,0x30,0x84,0x30,0x84,0x2f,0x7c,0x2f,0x7b,0xce,0x73,0xce,0x73,0x8d,0x73,0x8d,
0x6b,0x4d,0x6b,0x4d,0x6b,0x4c,0x63,0x0c,0x63,0x0c,0x62,0xcb,0x5a,0xcb,0x5a,0xcb,
0x52,0x8a,0x52,0x4a,0x4a,0x4a,0x4a,0x49,0x4a,0x09,0x42,0x08,0x42,0x08,0x41,0xc8,
0x39,0xc7,0x39,0xc7,0x31,0xc7,0x31,0x86,0x31,0x86,0x31,0x86,0x31,0x46,0x29,0x45,
0x29,0x45,0x29,0x45,0x29,0x45,0x29,0x45,0x21,0x05,0x29,0x45,0x94,0xb1,0xff,0xff,
0xff,0xff,0xff,0xff,0xce,0x79,0x94,0xb2,0x8c,0x71,0x8c,0x72,0x8c,0x71,0x84,0x31,
0x84,0x31,0x84,0x30,0x84,0x30,0x83,0xd0,0x7b,0xcf,0x7b,0x8f,0x73,0x8e,0x6b,0x8e,
0x6b,0x4d,0x6b,0x4d,0x63,0x4d,0x63,0x0c,0x63,0x0c,0x5b,0x0b,0x5b,0x0b,0x5a,0xcb,
0x5a,0xcb,0x5a,0xcb,0x5a,0xcb,0x5a,0xca,0x5a,0xca,0x52,0x8a,0x52,0x8a,0x52,0x8a,
0x52,0x8a,0x52,0x8a,0x52,0x8a,0x52,0x49,0x4a,0x49,0x4a,0x49,0x4a,0x49,0x4a,0x49,
0x39,0xc7,0x29,0x45,0x21,0x45,0x29,0x45,0x52,0x89,0xbd,0xb5,0xff,0xff,0xff,0xff
};
