const unsigned char manage[]={
/*����
Image size:48 X 48 X 16BPP=4608 bytes.
4 bytes=P(x,y)+P(x+1,y)
Data format: 4 bytes(2 pixels)=RRRRRGGG,GGGBBBBB,RRRRRGGG,GGGBBBBB
*/
0xff,0xff,0xff,0xff,0xff,0xff,0xe7,0x3c,0xad,0xb7,0x7c,0x33,0x74,0x32,0x63,0x91,
0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,
0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,
0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,
0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,
0x63,0x91,0x74,0x32,0x84,0x73,0xa5,0x76,0xef,0x7d,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xf7,0xbe,0xbe,0x39,0x6b,0xd2,0x63,0x91,0x63,0x91,0x74,0x32,0x8c,0xb4,
0xad,0xb8,0xad,0xb8,0xad,0xb8,0xad,0xb8,0xad,0xb8,0xad,0xb8,0xad,0xb8,0xad,0xb8,
0xad,0xb8,0xad,0xb8,0xad,0xb8,0xad,0xb8,0xad,0xb8,0xad,0xb8,0xad,0xb8,0xad,0xb8,
0xad,0xb8,0xad,0xb8,0xad,0xb8,0xad,0xb8,0xad,0xb8,0xad,0xb8,0xad,0xb8,0xad,0xb8,
0xad,0xb8,0xad,0xb8,0xad,0xb8,0xad,0xb8,0xad,0xb8,0xad,0xb8,0xad,0xb8,0xad,0xb8,
0x94,0xf5,0x6b,0xd1,0x63,0x91,0x6b,0xd1,0x8c,0xb4,0xce,0xbb,0xf7,0xbf,0xff,0xff,
0xf7,0xff,0xbe,0x39,0x6b,0xd1,0x63,0x91,0x7c,0x73,0xb5,0xf8,0xe7,0x7d,0xef,0x7d,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xef,0x7e,0xdf,0x3c,0xbe,0x39,0x74,0x32,0x63,0x91,0x7c,0x33,0xce,0xbb,0xff,0xff,
0xc6,0x7a,0x6b,0xd2,0x6b,0x91,0xa5,0x77,0xf7,0xbe,0xff,0xff,0xff,0xff,0xf7,0xbe,
0xbd,0xb9,0xa4,0xf7,0xad,0x38,0xa4,0xf7,0xa4,0xf8,0xac,0xf8,0xad,0x38,0xad,0x38,
0xb5,0x39,0xb5,0x38,0xb5,0x39,0xbd,0xb8,0xef,0x7e,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xd6,0xfc,0xb6,0x3a,0xbe,0x3a,0xb6,0x39,0xb6,0x39,
0xad,0xf9,0xa5,0xb8,0x9d,0xb8,0x9d,0x78,0x9d,0x78,0x9d,0x77,0x95,0x77,0x95,0x77,
0xb6,0x39,0xf7,0xbe,0xf7,0xbf,0xd6,0xfb,0x94,0xf5,0x63,0x91,0x8c,0xf5,0xe7,0x3d,
0x9d,0x36,0x63,0x91,0x95,0x36,0xf7,0xbe,0xff,0xff,0xff,0xff,0xf7,0xbf,0x83,0x94,
0x52,0x10,0x5a,0x10,0x5a,0x51,0x51,0xd0,0x41,0x4f,0x41,0x50,0x49,0x91,0x51,0xd2,
0x5a,0x13,0x62,0x13,0x72,0x94,0x9b,0xd7,0x8b,0xd3,0xef,0x7d,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xe7,0x7e,0xce,0xfd,0xbe,0xbc,0xbe,0xbc,0xb6,0x7b,0xb6,0x3b,
0xa5,0xfa,0x9d,0xb8,0x8d,0x78,0x85,0x38,0x85,0x37,0x84,0xf7,0x84,0xf7,0x8d,0x38,
0x95,0x78,0xc6,0xbb,0xff,0xff,0xff,0xff,0xd6,0xbb,0x7c,0x73,0x6b,0xd1,0x9d,0x36,
0x6b,0xd2,0x74,0x32,0xc6,0x7a,0xff,0xff,0xff,0xff,0xff,0xff,0xb5,0x79,0x51,0xcf,
0x52,0x10,0x5a,0x10,0x5a,0x10,0x62,0x51,0x49,0xd0,0x41,0x4f,0x49,0x91,0x49,0x91,
0x62,0x13,0x5a,0x13,0x5a,0x13,0x83,0x15,0xac,0x78,0x94,0xb3,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xd7,0x3d,0x95,0x77,0x74,0x74,0x74,0xb4,0x74,0x73,0x74,0x73,
0x63,0xd2,0x4b,0x0e,0x4b,0x0e,0x4b,0x0e,0x43,0x0e,0x4b,0x0e,0x4b,0x0e,0x4b,0x4f,
0x74,0xb5,0x9d,0xf9,0xff,0xff,0xff,0xff,0xf7,0xbf,0xc6,0x7a,0x63,0x91,0x74,0x32,
0x63,0x91,0x95,0x36,0xef,0xbe,0xff,0xff,0xff,0xff,0xff,0xff,0x8b,0xd5,0x51,0xcf,
0x52,0x0f,0x5a,0x10,0x5a,0x10,0x62,0x51,0x5a,0x11,0x41,0x4f,0x41,0x50,0x49,0x91,
0xce,0x3b,0xbd,0xba,0x59,0xd2,0x6a,0x53,0xa4,0x38,0x7b,0x91,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xce,0xfd,0x74,0x74,0x74,0xb4,0x74,0x74,0x74,0x73,0x6c,0x73,
0x63,0xd1,0x5b,0x4f,0x4b,0x0e,0x42,0xcd,0x3a,0x8c,0x3a,0xcd,0x43,0x0e,0x43,0x0f,
0x64,0x33,0x9d,0xb8,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7d,0x84,0xb4,0x6b,0x91,
0x63,0x91,0xa5,0x77,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x83,0x94,0x51,0xcf,
0x51,0xcf,0x52,0x10,0x5a,0x10,0x5a,0x11,0x62,0x51,0x41,0x50,0x39,0x0f,0x9c,0x77,
0xff,0xff,0xff,0xff,0xb5,0x39,0x62,0x13,0x9b,0xd7,0x7b,0x91,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xce,0xfc,0x74,0xb4,0x74,0xb4,0x74,0x73,0x74,0x32,0x63,0xd1,
0x6c,0x32,0xd6,0xfb,0x9d,0x77,0x32,0x4b,0x42,0xcd,0x3a,0xcd,0x43,0x0f,0x43,0x10,
0x64,0x33,0x9d,0xb9,0xff,0xff,0xff,0xff,0xff,0xff,0xf7,0xff,0xb5,0xf8,0x63,0x91,
0x63,0x91,0xce,0x7a,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x83,0x94,0x51,0xcf,
0x51,0xcf,0x52,0x10,0x5a,0x10,0x52,0x10,0x5a,0x10,0x39,0x0f,0x6a,0x93,0xff,0xbf,
0xff,0xff,0xff,0xff,0xac,0xf8,0x59,0xd2,0x8b,0x56,0x7b,0x91,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xce,0xfc,0x7c,0xb4,0x74,0xb4,0x84,0xb5,0xa5,0x77,0x7c,0xb3,
0x95,0x35,0xe7,0x3d,0xd6,0xfb,0x7c,0x31,0xbe,0x39,0xa5,0xb7,0x42,0xce,0x43,0x0f,
0x64,0x73,0x9d,0xf9,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x83,0x94,0x51,0xcf,
0x51,0xcf,0x51,0xcf,0x49,0xcf,0x6a,0xd2,0x94,0x36,0x6a,0xd2,0x83,0xd5,0xff,0xff,
0xff,0xff,0xcd,0xfb,0x59,0xd2,0x59,0xd3,0x83,0x16,0x7b,0x51,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xce,0xfd,0x7c,0xb5,0x74,0xb4,0x95,0x36,0xbe,0x3a,0xa5,0x77,
0x8c,0xf5,0xa5,0x77,0xbd,0xf8,0xd6,0xbb,0xf7,0xbf,0xbe,0x3a,0x4b,0x0d,0x43,0x0e,
0x64,0x33,0x9d,0xf9,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x83,0x94,0x51,0xcf,
0x51,0xcf,0x49,0x8e,0xa4,0xb6,0xff,0xff,0xf7,0xbe,0xf7,0xbe,0xff,0xfe,0xf7,0xbe,
0xd6,0x7c,0x62,0x53,0x59,0xd2,0x5a,0x13,0x72,0x95,0x73,0x10,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xce,0xfd,0x84,0xf5,0x7c,0xb4,0x7c,0x73,0xb6,0x3a,0x9d,0x77,
0x84,0xb4,0x9d,0x36,0xad,0xb8,0xad,0xb7,0xd6,0xbc,0x7c,0x72,0x3a,0x8b,0x43,0x0e,
0x64,0x32,0x9d,0xb9,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x83,0x94,0x51,0xcf,
0x49,0x8f,0x8b,0xd4,0xf7,0xbe,0x94,0x35,0x62,0x51,0x6a,0x92,0xc5,0xfa,0xde,0xfc,
0x49,0x91,0x51,0x91,0x5a,0x12,0x5a,0x13,0x6a,0x55,0x6b,0x10,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xd7,0x3d,0x84,0xf4,0x8d,0x36,0xb5,0xf9,0xc6,0x7b,0xad,0xb8,
0x8d,0x36,0x5b,0x4f,0x6b,0xd1,0x9d,0x77,0xad,0xb8,0xa5,0xb7,0x74,0x33,0x4b,0x0f,
0x64,0x32,0x9d,0xf9,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x7b,0x94,0x49,0xcf,
0x51,0xcf,0xde,0xfc,0x9c,0x76,0x49,0x8e,0x5a,0x10,0x49,0x8f,0x39,0x4f,0xe6,0xfd,
0x83,0x95,0x49,0x51,0x59,0xd2,0x5a,0x13,0x6a,0x54,0x6a,0xd0,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xd7,0x3c,0x84,0xb4,0xb6,0x39,0xff,0xff,0xd6,0xba,0xb5,0xf8,
0x7c,0xb4,0x4b,0x0f,0x3a,0x8d,0x74,0x33,0x95,0x36,0xbe,0x7b,0xbe,0x7b,0x4b,0x4f,
0x5c,0x32,0x9d,0xf9,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x7b,0x94,0x49,0x8e,
0x6a,0xd1,0xe7,0x3d,0x62,0x51,0x51,0xcf,0x5a,0x10,0x52,0x10,0x28,0x8d,0xac,0xf8,
0xb5,0x79,0x41,0x10,0x59,0xd2,0x5a,0x13,0x6a,0x54,0x6a,0xcf,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xd7,0x3d,0x84,0xb4,0xa5,0xb7,0xdf,0x3d,0xd6,0xbb,0xb5,0xf9,
0x84,0xb5,0x4b,0x4f,0x43,0x0e,0x6b,0xd2,0x84,0xb5,0xa5,0xb9,0x8d,0x36,0x43,0x0f,
0x64,0x32,0x9d,0xf9,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x7b,0x54,0x49,0x8e,
0x73,0x12,0xe6,0xfc,0x52,0x10,0x51,0xcf,0x5a,0x10,0x52,0x10,0x28,0x8d,0x94,0x76,
0xbd,0xba,0x41,0x10,0x51,0xd2,0x5a,0x12,0x6a,0x54,0x6a,0xcf,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xd6,0xfd,0x8c,0xf5,0x84,0xf5,0x84,0xb4,0xd6,0xbb,0xc6,0x79,
0xa5,0x77,0x63,0xd0,0x5b,0x90,0x6b,0xd2,0x8d,0x36,0x6c,0x32,0x3a,0xcd,0x43,0x0e,
0x64,0x73,0x9d,0xf9,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x7b,0x94,0x49,0x8e,
0x62,0x50,0xef,0x3d,0x6a,0xd2,0x49,0xcf,0x5a,0x10,0x51,0xd0,0x28,0x8d,0xbd,0xba,
0xa4,0xf8,0x39,0x0f,0x51,0xd1,0x59,0xd2,0x6a,0x14,0x6a,0xcf,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xce,0xfd,0x84,0xf5,0x84,0xf5,0x8d,0x36,0xe7,0x7d,0xe7,0x3d,
0xbd,0xf8,0x84,0xb3,0x6b,0xd2,0x84,0xb4,0x9d,0x77,0x74,0x33,0x43,0x0e,0x43,0x4f,
0x64,0x33,0x9d,0xf9,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x7b,0x94,0x49,0xcf,
0x49,0x8e,0xc5,0xfa,0xc6,0x3a,0x49,0x8f,0x49,0x8f,0x41,0x4e,0x62,0x92,0xf7,0xbe,
0x62,0x52,0x41,0x10,0x51,0x91,0x59,0xd2,0x62,0x14,0x62,0xcf,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xce,0xfd,0x84,0xb4,0x7c,0xb4,0x9d,0x77,0xe7,0xbe,0xce,0xba,
0xbe,0x39,0x9d,0x77,0x8c,0xf5,0x84,0xb4,0x94,0xf5,0x84,0xf4,0x4b,0x0e,0x43,0x0f,
0x64,0x73,0x9d,0xf9,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x7b,0x94,0x49,0xcf,
0x49,0x8e,0x62,0x91,0xef,0x7d,0xd6,0xbb,0x94,0x76,0xad,0x38,0xff,0xff,0x9c,0xb7,
0x30,0xce,0x41,0x50,0x49,0x91,0x51,0xd2,0x62,0x14,0x62,0xcf,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xce,0xfd,0x7c,0xb4,0x7c,0xb5,0x7c,0xb4,0x8c,0xf5,0x74,0x31,
0x84,0xb3,0xb6,0x39,0x84,0xf5,0x42,0xcd,0x53,0x4e,0x53,0x4e,0x43,0x0e,0x43,0x4f,
0x64,0x33,0x9d,0xb8,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x7b,0x94,0x49,0xcf,
0x51,0xcf,0x49,0x8e,0x62,0x91,0xc5,0xfa,0xef,0x7d,0xde,0xfc,0x83,0x94,0x30,0xce,
0x39,0x0f,0x41,0x50,0x49,0x90,0x51,0xd2,0x62,0x14,0x62,0xcf,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xce,0xfd,0x74,0x74,0x74,0xb4,0x74,0x74,0x6c,0x33,0x6c,0x32,
0x63,0xd1,0x6c,0x31,0x53,0x50,0x3a,0xcd,0x3a,0xcd,0x3a,0xcd,0x43,0x0e,0x43,0x0f,
0x64,0x33,0x9d,0xb8,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xb5,0xf9,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x9c,0x76,0x49,0xcf,
0x49,0xcf,0x51,0xcf,0x49,0x8f,0x49,0x8e,0x5a,0x10,0x41,0x8f,0x20,0x8c,0x39,0x0e,
0x39,0x0f,0x41,0x4f,0x49,0x90,0x51,0x91,0x62,0x14,0x73,0x50,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xce,0xfd,0x7c,0xb4,0x6c,0x33,0x74,0x73,0x6c,0x33,0x6c,0x33,
0x5b,0x91,0x42,0xce,0x42,0xcd,0x42,0xcd,0x3a,0x8c,0x3a,0xcd,0x42,0xcd,0x43,0x0e,
0x6c,0x73,0x9d,0xb8,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xb5,0xf9,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xde,0xbc,0x52,0x10,
0x49,0xcf,0x51,0xcf,0x51,0xcf,0x51,0xcf,0x52,0x10,0x41,0x8f,0x28,0xcd,0x31,0x0e,
0x39,0x0f,0x41,0x4f,0x49,0x50,0x51,0xd2,0x49,0x8f,0xb5,0xb6,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xe7,0x7e,0xbe,0x7b,0x9d,0xb9,0x9d,0xb8,0x9d,0xb8,0x95,0x78,
0x8d,0x36,0x7c,0xb5,0x74,0x74,0x6c,0x74,0x6c,0x33,0x6c,0x34,0x6c,0x74,0x74,0xb5,
0x8d,0x77,0xbe,0x7b,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xb5,0xf9,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc5,0xf9,
0x7b,0x53,0x73,0x53,0x7b,0x53,0x7b,0x53,0x7b,0x53,0x73,0x12,0x62,0x92,0x6a,0xd2,
0x6a,0xd2,0x6a,0xd3,0x73,0x13,0x6a,0xd2,0xa5,0x35,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xb5,0xf9,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xb5,0xf9,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xb5,0xf9,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xe7,0x3c,0xad,0xb5,0xc6,0x38,0xc6,0x39,0xc6,0x38,
0xc6,0x38,0xc6,0x38,0xc6,0x38,0xc6,0x38,0xc6,0x38,0xbd,0xf7,0xce,0x79,0xc5,0xf7,
0xc5,0xf7,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xb5,0xf9,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xf7,0xbe,0xad,0x75,0x94,0x72,0x84,0x30,0x7b,0xcf,0x73,0x8e,
0x6b,0x4d,0x63,0x0c,0x5a,0xcb,0x4a,0x49,0x42,0x08,0x39,0xc7,0x31,0x86,0x29,0x46,
0x21,0x44,0x9c,0xb2,0xef,0x7d,0xff,0xff,0xff,0xff,0xff,0xff,0xb5,0xf9,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xef,0x7d,0xef,0x7d,0xef,0x7d,0xef,0x7d,0xef,0x7d,0xef,0x7d,0xef,0x7d,0xef,0x7d,
0xef,0x7d,0xef,0x7d,0xef,0x7d,0xef,0x7d,0xef,0x7d,0xf7,0xbe,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xd6,0xba,0x94,0xb2,0x8c,0x71,0x84,0x30,0x7c,0x30,0x73,0x8e,
0x5a,0xcb,0x42,0x09,0x39,0xc8,0x42,0x08,0x42,0x08,0x39,0xc7,0x31,0x86,0x29,0x45,
0x29,0x45,0x42,0x07,0xb5,0x75,0xff,0xff,0xff,0xff,0xff,0xff,0xb5,0xf9,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf7,0xbe,
0xef,0x7d,0xf7,0x7d,0xf7,0x7d,0xef,0x7d,0xef,0x7d,0xef,0x7d,0xef,0x7d,0xef,0x7d,
0xef,0x7d,0xef,0x7d,0xef,0x7d,0xef,0xbe,0xef,0x7d,0xef,0x7d,0xf7,0xbe,0xff,0xff,
0xff,0xff,0xff,0xff,0xbd,0xf7,0x94,0xb2,0x8c,0x71,0x84,0x30,0x73,0x8e,0x39,0xc7,
0x29,0x45,0x31,0x86,0x39,0xc6,0x29,0x45,0x29,0x45,0x31,0x86,0x31,0x86,0x29,0x45,
0x29,0x45,0x39,0xc7,0x9c,0xf3,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7d,
0xef,0x7d,0xce,0xfb,0xde,0xfb,0xf7,0xbd,0xef,0x7d,0xef,0x7d,0xef,0x7d,0xef,0x7d,
0xef,0x7d,0xef,0x7d,0xef,0xbe,0xde,0x78,0xe7,0x3c,0xef,0x7d,0xef,0x7d,0xff,0xff,
0xff,0xff,0xff,0xff,0xbd,0xf7,0x94,0xb2,0x94,0x71,0x7b,0xcf,0x31,0x86,0x42,0x07,
0x8c,0x2e,0xac,0xf0,0xad,0x30,0x94,0xaf,0x5a,0x89,0x29,0x45,0x29,0x45,0x29,0x45,
0x29,0x45,0x39,0xc7,0x9c,0xf3,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7d,
0xff,0xbd,0x9e,0x3b,0x44,0xfa,0xd6,0xfc,0xff,0xbd,0xef,0x7d,0xef,0x7d,0xef,0x7d,
0xef,0xbe,0xef,0xff,0xdd,0x34,0xd2,0x89,0xe6,0xfb,0xef,0xbe,0xef,0x7d,0xff,0xff,
0xff,0xff,0xff,0xff,0xbe,0x37,0x94,0xb2,0x8c,0xb1,0x42,0x08,0x42,0x08,0xbd,0xb4,
0xd6,0xb5,0xe6,0xb6,0xe6,0x75,0xde,0xb6,0xbd,0xb2,0x6b,0x4b,0x29,0x45,0x29,0x45,
0x29,0x45,0x39,0xc7,0xa4,0xf3,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7d,
0xff,0xbd,0xa6,0x7b,0x04,0x7a,0x34,0xfa,0xce,0xbb,0xff,0xbd,0xef,0x7d,0xef,0xbe,
0xef,0xff,0xd4,0xf2,0xd9,0x02,0xd1,0xc7,0xe7,0x3c,0xef,0xbe,0xef,0x7d,0xff,0xff,
0xff,0xff,0xff,0xff,0xc6,0x37,0x9c,0xf2,0x6b,0x8d,0x29,0x45,0xad,0x72,0xef,0x39,
0xe6,0xf6,0xef,0x37,0xf7,0x78,0xe6,0xf6,0xe6,0xf6,0xce,0x34,0x4a,0x48,0x21,0x05,
0x29,0x45,0x39,0xc7,0xa5,0x33,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7d,
0xff,0xbd,0xae,0x7b,0x0c,0xbb,0x04,0x7b,0x2c,0xfa,0xb6,0x7b,0xf7,0xbe,0xe7,0x7d,
0xd4,0x70,0xd1,0x02,0xd8,0x81,0xd2,0x48,0xe7,0x3c,0xef,0xbe,0xef,0x7d,0xff,0xff,
0xff,0xff,0xff,0xff,0xc6,0x37,0x9c,0xf3,0x4a,0x49,0x5a,0xca,0xde,0xb7,0xef,0x39,
0xf7,0x78,0xde,0xb5,0xce,0x34,0xf7,0x37,0xef,0x37,0xd6,0x75,0x8c,0x6e,0x29,0x45,
0x21,0x45,0x39,0xc6,0x9c,0xf2,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7d,
0xff,0xbd,0xae,0x7b,0x0c,0xbb,0x04,0xbb,0x04,0xbb,0x1c,0xba,0xbe,0xbb,0xe4,0x30,
0xd0,0x40,0xd8,0xc2,0xd8,0xc2,0xd2,0x48,0xe7,0x3c,0xef,0xbe,0xef,0x7d,0xff,0xff,
0xff,0xff,0xff,0xff,0xc6,0x38,0x94,0xb2,0x39,0xc7,0x94,0x6f,0xf7,0x7a,0xf7,0x7a,
0xff,0x78,0xd6,0x74,0xb5,0x72,0xf7,0x37,0xef,0x37,0xee,0xf7,0xbd,0xb2,0x39,0xc7,
0x21,0x04,0x39,0xc7,0xa4,0xf3,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7d,
0xff,0xbd,0xae,0x7b,0x0c,0xbb,0x04,0xbb,0x04,0x7b,0x5d,0x7a,0xe7,0x3c,0xed,0xf6,
0xd1,0xc6,0xd8,0x81,0xd8,0xc2,0xd2,0x48,0xe7,0x3c,0xef,0xbe,0xef,0x7d,0xff,0xff,
0xff,0xff,0xff,0xff,0xc6,0x38,0x8c,0x71,0x31,0xc7,0xac,0xf1,0xee,0xf8,0xde,0xf7,
0xde,0xb6,0xb5,0x72,0xa4,0xf0,0xf7,0x37,0xef,0x37,0xf7,0x37,0xcd,0xf3,0x4a,0x48,
0x21,0x04,0x39,0xc7,0xa4,0xf3,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7d,
0xff,0xbd,0xae,0x7b,0x0c,0x7b,0x04,0x7b,0x7d,0xba,0xe7,0x3b,0x85,0xba,0xa6,0xbc,
0xee,0xb9,0xda,0x48,0xd8,0x80,0xd2,0x08,0xe7,0x3c,0xef,0xbe,0xef,0x7d,0xff,0xff,
0xff,0xff,0xff,0xff,0xc6,0x38,0x84,0x30,0x39,0xc7,0xac,0xf1,0xee,0xb7,0xc6,0x35,
0xc5,0xf4,0xad,0x31,0x8c,0x2e,0xee,0xf7,0xef,0x37,0xef,0x37,0xcd,0xb2,0x52,0x49,
0x21,0x04,0x39,0x86,0x9c,0xb2,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7d,
0xff,0xbd,0xa6,0x3b,0x0c,0x7a,0x8d,0xfa,0xef,0x3b,0x65,0x7a,0x04,0x7b,0x14,0xba,
0x9e,0x7c,0xee,0xfb,0xda,0xca,0xd1,0xc6,0xe7,0x3c,0xef,0xbe,0xef,0x7d,0xff,0xff,
0xff,0xff,0xff,0xff,0xc6,0x38,0x8c,0x71,0x31,0x87,0x9c,0xf1,0xf7,0x79,0xf7,0x7a,
0xef,0x38,0xf7,0x37,0xd6,0x75,0xc5,0xf3,0xef,0x37,0xf7,0x38,0xc5,0xf3,0x42,0x07,
0x21,0x04,0x39,0x86,0x9c,0xb2,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7d,
0xf7,0xbd,0xae,0x7b,0xa6,0x3a,0xe6,0xfb,0x55,0x3a,0x04,0x7b,0x04,0xbb,0x04,0xbb,
0x0c,0x7a,0x86,0x3b,0xee,0xfa,0xdc,0x70,0xe6,0xfb,0xef,0xbe,0xef,0x7d,0xff,0xff,
0xff,0xff,0xff,0xff,0xc6,0x38,0x94,0xb2,0x3a,0x08,0x7b,0x8d,0xde,0xf7,0xef,0x39,
0xf7,0x7a,0xef,0x37,0xf7,0x78,0xd6,0x34,0xcd,0xf3,0xe6,0xb6,0xa4,0xf0,0x31,0x86,
0x21,0x05,0x39,0xc6,0x9c,0xb2,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7d,
0xef,0x7d,0xf7,0x7c,0xce,0xbb,0x3c,0xfa,0x04,0x7b,0x04,0xbb,0x04,0xbb,0x04,0xbb,
0x04,0xbb,0x04,0x7a,0x75,0xbb,0xe7,0x3c,0xef,0x7c,0xef,0x7d,0xef,0x7d,0xff,0xff,
0xff,0xff,0xff,0xff,0xc6,0x38,0x9c,0xf3,0x5a,0xcb,0x41,0xc7,0xce,0x35,0xef,0x39,
0xef,0x39,0xef,0x38,0xef,0x37,0xef,0x37,0xce,0x34,0xce,0x34,0x6b,0x4b,0x21,0x05,
0x29,0x45,0x39,0xc7,0xa5,0x34,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xde,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7d,
0xf7,0xbd,0xce,0xbb,0x2c,0xfa,0x04,0x7b,0x04,0xbb,0x04,0xbb,0x34,0xfa,0x24,0xfa,
0x04,0xbb,0x04,0xbb,0x04,0x7b,0x6d,0xba,0xef,0x7d,0xef,0x7d,0xef,0x7d,0xff,0xff,
0xff,0xff,0xff,0xff,0xbd,0xf7,0x94,0xb2,0x84,0x31,0x29,0x46,0x73,0x8c,0xde,0xb7,
0xde,0xb7,0xf7,0x79,0xf7,0x38,0xde,0xb5,0xd6,0x74,0x9c,0xaf,0x31,0x86,0x29,0x45,
0x29,0x45,0x39,0xc7,0xa5,0x34,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xd6,0xfc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7d,
0xff,0xbd,0x9e,0x3b,0x04,0x7a,0x04,0xbb,0x04,0x7b,0x55,0x7a,0xe7,0x3b,0xbe,0x7b,
0x24,0xba,0x04,0x7b,0x04,0xbb,0x24,0xfa,0xe7,0x3c,0xf7,0x7d,0xef,0x7d,0xff,0xff,
0xff,0xff,0xff,0xff,0xbd,0xf7,0x94,0xb1,0x94,0xb2,0x63,0x0c,0x21,0x04,0x73,0x8d,
0xce,0x35,0xde,0xb7,0xe6,0x76,0xce,0x75,0x8c,0x2e,0x39,0xc6,0x29,0x45,0x29,0x45,
0x29,0x45,0x39,0xc7,0xa5,0x34,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0xc6,0x7a,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7d,
0xff,0xbd,0xa6,0x3b,0x04,0x7b,0x04,0x7b,0x6d,0x7a,0xef,0x3c,0xf7,0xbd,0xff,0xbd,
0xce,0xbb,0x34,0xfa,0x04,0x7b,0x2c,0xfa,0xe7,0x3c,0xf7,0x7d,0xef,0x7d,0xff,0xff,
0xff,0xff,0xff,0xff,0xbd,0xf7,0x94,0xb2,0x8c,0x71,0x8c,0x71,0x5a,0xcb,0x29,0x45,
0x42,0x07,0x6b,0x4c,0x73,0x8d,0x5a,0xca,0x31,0x86,0x29,0x45,0x31,0x86,0x29,0x45,
0x29,0x45,0x42,0x08,0xad,0x75,0xff,0xff,0xff,0xff,0xff,0xff,0xbe,0x39,0x63,0x91,
0x63,0x91,0x9d,0x76,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7d,
0xff,0xbd,0x96,0x3b,0x04,0x7a,0x7d,0xba,0xf7,0x7c,0xf7,0xbd,0xef,0x7d,0xef,0x7d,
0xff,0xbd,0xde,0xfb,0x44,0xfa,0x24,0xba,0xe7,0x3c,0xf7,0x7d,0xef,0x7d,0xff,0xff,
0xff,0xff,0xff,0xff,0xbe,0x37,0x94,0xb2,0x8c,0x71,0x84,0x30,0x7c,0x30,0x63,0x0c,
0x39,0xc8,0x29,0x46,0x29,0x45,0x29,0x46,0x39,0xc7,0x39,0xc7,0x31,0x86,0x29,0x45,
0x29,0x45,0x4a,0x49,0xc6,0x37,0xff,0xff,0xff,0xff,0xf7,0xbe,0xad,0xb7,0x63,0x91,
0x63,0x91,0x8c,0xf5,0xef,0x7d,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xef,0x7d,
0xf7,0x7d,0xa6,0x3a,0x8d,0xfa,0xf7,0x7d,0xf7,0xbd,0xef,0x7d,0xef,0x7d,0xef,0x7d,
0xef,0x7d,0xff,0xbd,0xde,0xfc,0x7d,0xb9,0xde,0xfc,0xf7,0x7d,0xef,0x7d,0xff,0xff,
0xff,0xff,0xff,0xff,0xe7,0x3c,0x94,0xb2,0x8c,0x71,0x84,0x30,0x7b,0xcf,0x73,0xce,
0x6b,0x4d,0x5a,0xcb,0x52,0x8a,0x4a,0x49,0x42,0x08,0x39,0xc7,0x31,0x86,0x29,0x45,
0x21,0x05,0x73,0x8e,0xe6,0xfb,0xff,0xff,0xff,0xff,0xdf,0x3c,0x74,0x32,0x6b,0xd1,
0x74,0x32,0x6b,0xd1,0xb5,0xf8,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf7,0xbe,
0xef,0x7d,0xe7,0x3c,0xf7,0x7d,0xf7,0xbd,0xef,0x7d,0xef,0x7d,0xef,0x7d,0xef,0x7d,
0xef,0x7d,0xef,0x7d,0xf7,0xbd,0xef,0x3c,0xe7,0x3c,0xf7,0xbe,0xf7,0xbe,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xc6,0x38,0xb5,0x76,0xad,0x75,0xa5,0x75,0x9d,0x34,
0x9c,0xf3,0x94,0xf3,0x94,0xb3,0x9c,0xf3,0x94,0xb2,0x94,0xb2,0x9c,0xf3,0x83,0xcf,
0x8c,0x2f,0xef,0x7c,0xff,0xff,0xff,0xff,0xf7,0xbe,0xb5,0xf8,0x63,0x91,0x84,0x73,
0xad,0xb7,0x63,0x91,0x84,0xb4,0xdf,0x3c,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xf7,0xbe,0xf7,0xbe,0xf7,0xbe,0xf7,0xbe,0xf7,0xbe,0xf7,0xbe,0xf7,0xbe,0xf7,0xbe,
0xf7,0xbe,0xf7,0xbe,0xf7,0xbe,0xf7,0xbe,0xf7,0xbe,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xf7,0xbe,0xbe,0x39,0x6b,0xd1,0x6b,0xd2,0xad,0xb7,
0xde,0xfc,0x84,0x73,0x63,0x91,0x7c,0x33,0xb5,0xb8,0xef,0xbe,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xf7,0xff,0xe7,0x7d,0xb5,0xf8,0x6b,0xd2,0x6b,0xd1,0xa5,0x77,0xf7,0xbe,
0xff,0xff,0xd6,0xfb,0x7c,0x73,0x63,0x91,0x6b,0xd1,0x94,0xf5,0xc6,0x7a,0xce,0x7a,
0xce,0xbb,0xce,0xbb,0xce,0xbb,0xce,0xba,0xce,0xba,0xce,0x7a,0xce,0x7a,0xce,0x7a,
0xc6,0x7a,0xc6,0x7a,0xc6,0x7a,0xc6,0x7a,0xc6,0x7a,0xc6,0x7a,0xc6,0x7a,0xc6,0x7a,
0xc6,0x7a,0xc6,0x7a,0xc6,0x7a,0xc6,0x7a,0xc6,0x7a,0xc6,0x7a,0xc6,0x7a,0xce,0x7a,
0xce,0x7a,0xce,0x7a,0xce,0x7a,0xce,0xba,0xce,0xbb,0xce,0xba,0xce,0xba,0xce,0xba,
0xce,0x7a,0xb5,0xf8,0x7c,0x73,0x63,0x91,0x6b,0xd1,0xa5,0x76,0xef,0x7e,0xff,0xff,
0xff,0xff,0xff,0xff,0xe7,0x7d,0xad,0xb8,0x73,0xd2,0x63,0x91,0x63,0x91,0x63,0x91,
0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,
0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,
0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,
0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,0x63,0x91,
0x63,0x91,0x63,0x91,0x6b,0x91,0x7c,0x33,0xad,0xb7,0xf7,0xbe,0xff,0xff,0xff,0xff
};
