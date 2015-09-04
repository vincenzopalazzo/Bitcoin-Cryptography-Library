/* 
 * A runnable main program that tests the functionality of class Uint256.
 * 
 * Bitcoin cryptography library
 * Copyright (c) Project Nayuki
 * 
 * http://www.nayuki.io/page/bitcoin-cryptography-library
 * https://github.com/nayuki/Bitcoin-Cryptography-Library
 */

#undef NDEBUG
#include <cstdint>
#include <cstdio>
#include <cstring>
#include "TestHelper.hpp"
#include "Uint256.hpp"


struct BinaryCase {
	const char *x;
	const char *y;
};

struct TernaryCase {
	const char *x;
	const char *y;
	const char *z;
	unsigned int bit;  // 0 or 1
};


static int numTestCases = 0;


static void testComparison() {
	BinaryCase cases[] = {  // All hexadecimal strings must be in lowercase for strcmp() to work properly
		{"0000000000000000000000000000000000000000000000000000000000000000", "0000000000000000000000000000000000000000000000000000000000000000"},
		{"0000000000000000000000000000000000000000000000000000000000000000", "0000000000000000000000000000000000000000000000000000000000000001"},
		{"0000000000000000000000000000000000000000000000000000000000000080", "0000000000000000000000000000000000000000000000000000000000000000"},
		{"ff00000000000000000000000000000000000000000000000000000000000000", "0000000000000000000000000000000000000000000000000000000000000000"},
		{"f000000000000000000000000000000000000000000000000000000000000000", "0000000000000000000000000000000000000000000000000000000000000000"},
		{"0123400000000000000000000000000000000000000000000000000000000000", "0123400000000000000000000000000000000000000000000000000000000000"},
		{"0000000000000000000000000000000000000000000000000000000000000000", "fffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f"},
	};
	for (unsigned int i = 0; i < ARRAY_LENGTH(cases); i++) {
		BinaryCase &tc = cases[i];
		int cmp = strcmp(tc.x, tc.y);
		Uint256 x(tc.x);
		Uint256 y(tc.y);
		assert((x == y) == (cmp == 0));
		assert((x != y) == (cmp != 0));
		assert((x <  y) == (cmp <  0));
		assert((x >  y) == (cmp >  0));
		assert((x <= y) == (cmp <= 0));
		assert((x >= y) == (cmp >= 0));
		numTestCases++;
	}
}


static void testAdd() {
	TernaryCase cases[] = {
		{"0000000000000000000000000000000000000000000000000000000000000000", "0000000000000000000000000000000000000000000000000000000000000000", "0000000000000000000000000000000000000000000000000000000000000000", 0},
		{"0000000000000000000000000000000000000000000000000000000080000000", "0000000000000000000000000000000000000000000000000000000080000000", "0000000000000000000000000000000000000000000000000000000100000000", 0},
		{"000000000000000000000000000000000000000000000fffffffffffffffffff", "0000000000000000000000000000000000000000000000000000000000000001", "0000000000000000000000000000000000000000000010000000000000000000", 0},
		{"fffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2e", "0000000000000000000000000000000000000000000000000000000000000001", "fffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f", 0},
		{"ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", "0000000000000000000000000000000000000000000000000000000000000001", "0000000000000000000000000000000000000000000000000000000000000000", 1},
		{"ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe", 1},
		{"ff00000000000000000000000000000000000000000000000000000000000000", "ff00000000000000000000000000000000000000000000000000000000000000", "fe00000000000000000000000000000000000000000000000000000000000000", 1},
	};
	for (unsigned int i = 0; i < ARRAY_LENGTH(cases); i++) {
		TernaryCase &tc = cases[i];
		Uint256 x(tc.x);
		Uint256 y(tc.y);
		assert(x.add(y, 0) == 0);
		assert(x == Uint256(tc.x));
		assert(x.add(y, 1) == tc.bit);
		assert(x == Uint256(tc.z));
		numTestCases++;
	}
}


static void testSubtract() {
	TernaryCase cases[] = {
		{"0000000000000000000000000000000000000000000000000000000000000000", "0000000000000000000000000000000000000000000000000000000000000000", "0000000000000000000000000000000000000000000000000000000000000000", 0},
		{"0000000000000000000000000000000000000000000000000000000000000003", "0000000000000000000000000000000000000000000000000000000000000002", "0000000000000000000000000000000000000000000000000000000000000001", 0},
		{"0000000000000000000000000000000000000000000000000000000000000000", "0000000000000000000000000000000000000000000000000000000000000001", "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", 1},
		{"0000000000000000000000000000000000000000000000000000000000000000", "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", "0000000000000000000000000000000000000000000000000000000000000001", 1},
	};
	for (unsigned int i = 0; i < ARRAY_LENGTH(cases); i++) {
		TernaryCase &tc = cases[i];
		Uint256 x(tc.x);
		Uint256 y(tc.y);
		assert(x.subtract(y, 0) == 0);
		assert(x == Uint256(tc.x));
		assert(x.subtract(y, 1) == tc.bit);
		assert(x == Uint256(tc.z));
		numTestCases++;
	}
}


static void testShiftLeft1() {
	TernaryCase cases[] = {
		{"0000000000000000000000000000000000000000000000000000000000000000", "0000000000000000000000000000000000000000000000000000000000000000", "", 0},
		{"0000000000000000000000000000000000000000000000000000000000000001", "0000000000000000000000000000000000000000000000000000000000000002", "", 0},
		{"0000000000000000000000000000000000000000000000000000000080000000", "0000000000000000000000000000000000000000000000000000000100000000", "", 0},
		{"00000000000000000000000000000000000000000000000000000000ffffffff", "00000000000000000000000000000000000000000000000000000001fffffffe", "", 0},
		{"000abcdef0000000000000000000000000000000000000000000000000000000", "001579bde0000000000000000000000000000000000000000000000000000000", "", 0},
		{"09f4be4173c99d20a7ac925a3ed6e95a3d2efa730b4bf7956168f2dea24bfc27", "13e97c82e7933a414f5924b47dadd2b47a5df4e61697ef2ac2d1e5bd4497f84e", "", 0},
		{"20e44f4466b9bdf9b599eef505df252b7022ffd23a8ebbeb547d12948492345f", "41c89e88cd737bf36b33ddea0bbe4a56e045ffa4751d77d6a8fa2529092468be", "", 0},
		{"44be7e70ef2d3fd4e40eb8f516f9ed6ccfdffe5e70162528077cbbfe5b24199f", "897cfce1de5a7fa9c81d71ea2df3dad99fbffcbce02c4a500ef977fcb648333e", "", 0},
		{"8288ba5463d3a027f5bcc6a05fd1d4a1b3c0b0e54b9750cf6c622f90a15e5b66", "051174a8c7a7404feb798d40bfa3a943678161ca972ea19ed8c45f2142bcb6cc", "", 1},
		{"8000000000000000000000000000000000000000000000000000000000000000", "0000000000000000000000000000000000000000000000000000000000000000", "", 1},
		{"ffff000000000000000000000000000000000000000000000000000000000000", "fffe000000000000000000000000000000000000000000000000000000000000", "", 1},
		{"ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe", "", 1},
	};
	for (unsigned int i = 0; i < ARRAY_LENGTH(cases); i++) {
		TernaryCase &tc = cases[i];
		Uint256 x(tc.x);
		assert(x.shiftLeft1() == tc.bit);
		assert(x == Uint256(tc.y));
		numTestCases++;
	}
}


static void testShiftRight1() {
	BinaryCase cases[] = {
		{"0000000000000000000000000000000000000000000000000000000000000000", "0000000000000000000000000000000000000000000000000000000000000000"},
		{"0000000000000000000000000000000000000000000000000000000000000001", "0000000000000000000000000000000000000000000000000000000000000000"},
		{"0000000000000000000000000000000000000000000000000000000100000000", "0000000000000000000000000000000000000000000000000000000080000000"},
		{"00000000000000000000000000000000000000000000000000000000ffffffff", "000000000000000000000000000000000000000000000000000000007fffffff"},
		{"0d659f38b34258b59e95248e384ec1c13280c3ae4427f6f86a7bba35979b789f", "06b2cf9c59a12c5acf4a92471c2760e0994061d72213fb7c353ddd1acbcdbc4f"},
		{"58c8e14f273f86a7ee70b95abf8bd7057c50837ff6f4885a822032d571e62ed8", "2c6470a7939fc353f7385cad5fc5eb82be2841bffb7a442d4110196ab8f3176c"},
		{"d5597aca79b909e0455b908aef1f060fe213f6832d7b9ba28f3a2572b0ce3d00", "6aacbd653cdc84f022adc845778f8307f109fb4196bdcdd1479d12b958671e80"},
		{"da5dc928fa00d056242652dad1b25d01d8747e0a32a4c99fcab44aff4a2a4b26", "6d2ee4947d00682b1213296d68d92e80ec3a3f05195264cfe55a257fa5152593"},
		{"000abcdef0000000000000000000000000000000000000000000000000000000", "00055e6f78000000000000000000000000000000000000000000000000000000"},
		{"ffff000000000000000000000000000000000000000000000000000000000000", "7fff800000000000000000000000000000000000000000000000000000000000"},
		{"ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", "7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"},
	};
	for (unsigned int i = 0; i < ARRAY_LENGTH(cases); i++) {
		BinaryCase &tc = cases[i];
		Uint256 x(tc.x);
		x.shiftRight1(0);
		assert(x == Uint256(tc.x));
		x.shiftRight1(1);
		assert(x == Uint256(tc.y));
		numTestCases++;
	}
}


static void testReciprocal() {
	TernaryCase cases[] = {
		{"0000000000000000000000000000000000000000000000000000000000000006", "000000000000000000000000000000000000000000000000000000000000000b", "0000000000000000000000000000000000000000000000000000000000000002", 0},
		{"000000000000000000000000000000000000000000000000000000000000000a", "000000000000000000000000000000000000000000000000000000000000000b", "000000000000000000000000000000000000000000000000000000000000000a", 0},
		{"000000000000000000000000000000000000000000000000000000000000000a", "000000000000000000000000000000000000000000000000000000000000000b", "000000000000000000000000000000000000000000000000000000000000000a", 0},
		{"0000000000000000000000000000000000000000000000000000000000226264", "0000000000000000000000000000000000000000000000000000000000bcd627", "00000000000000000000000000000000000000000000000000000000008909a4", 0},
		{"000000000000000000000000000000000000000000000000000000000054b08a", "0000000000000000000000000000000000000000000000000000000000bcd627", "00000000000000000000000000000000000000000000000000000000001d5b53", 0},
		{"000000000000000000000000000000000000000000000000000000000059fc3d", "0000000000000000000000000000000000000000000000000000000005cf4103", "000000000000000000000000000000000000000000000000000000000083034f", 0},
		{"000000000000000000000000000000000000000000000000000000000069e33c", "0000000000000000000000000000000000000000000000000000000000bcd627", "00000000000000000000000000000000000000000000000000000000002cf8d0", 0},
		{"0000000000000000000000000000000000000000000000000000000001c0abcc", "0000000000000000000000000000000000000000000000000000000005cf4103", "00000000000000000000000000000000000000000000000000000000008c5c37", 0},
		{"0000000000000000000000000000000000000000000000000000000004cd5694", "0000000000000000000000000000000000000000000000000000000005cf4103", "000000000000000000000000000000000000000000000000000000000359d9da", 0},
		{"0000000000000000000000000000000000000000000000000000003ac01b70a8", "0000000000000000000000000000000000000000000000000000009d4bbed6bb", "000000000000000000000000000000000000000000000000000000698b8cbc44", 0},
		{"0000000000000000000000000000000000000000000000000000006123d35703", "0000000000000000000000000000000000000000000000000000009d4bbed6bb", "0000000000000000000000000000000000000000000000000000001fe6c16a14", 0},
		{"0000000000000000000000000000000000000000000000000000008618728dc5", "0000000000000000000000000000000000000000000000000000009d4bbed6bb", "0000000000000000000000000000000000000000000000000000002ebe4d800f", 0},
		{"0000000000000000000000000000000000000000000000000000182e3d1e491a", "0000000000000000000000000000000000000000000000000000326146180d77", "000000000000000000000000000000000000000000000000000016ccd1557b20", 0},
		{"00000000000000000000000000000000000000000000000000001e49410d720c", "0000000000000000000000000000000000000000000000000000326146180d77", "00000000000000000000000000000000000000000000000000002527e2d7ba3a", 0},
		{"0000000000000000000000000000000000000000000000000000303881c9b0f2", "0000000000000000000000000000000000000000000000000000326146180d77", "0000000000000000000000000000000000000000000000000000138c6f73f28c", 0},
		{"0000000000000000000000000000000000000000000000000001149e23b1f140", "00000000000000000000000000000000000000000000000000018432130e284b", "00000000000000000000000000000000000000000000000000017bdc9a14ab1b", 0},
		{"00000000000000000000000000000000000000000000000000015e05404a0aff", "00000000000000000000000000000000000000000000000000018432130e284b", "00000000000000000000000000000000000000000000000000002896a661893e", 0},
		{"00000000000000000000000000000000000000000000000000016913d1359d4b", "00000000000000000000000000000000000000000000000000018432130e284b", "000000000000000000000000000000000000000000000000000101915a99c0c3", 0},
		{"0000000000000000000000000000000000000000000000000d1c47f15583f153", "000000000000000000000000000000000000000000000000172fb87a54d4742b", "000000000000000000000000000000000000000000000000051d78c33f75d885", 0},
		{"0000000000000000000000000000000000000000000000000decee03e1f793f5", "000000000000000000000000000000000000000000000000172fb87a54d4742b", "00000000000000000000000000000000000000000000000005111ab75abf38d8", 0},
		{"0000000000000000000000000000000000000000000000000ed4e69dd06af769", "000000000000000000000000000000000000000000000000172fb87a54d4742b", "00000000000000000000000000000000000000000000000015a0228b7fbd0741", 0},
		{"00000000000000000000000000000000000000000000000295bd93e503a1a2cd", "0000000000000000000000000000000000000000000000151ad982681c5191db", "000000000000000000000000000000000000000000000003da515835ede29f5a", 0},
		{"000000000000000000000000000000000000000000000008ee9bebe218118bf7", "0000000000000000000000000000000000000000000000151ad982681c5191db", "00000000000000000000000000000000000000000000000d1a466a3415144c2e", 0},
		{"000000000000000000000000000000000000000000000010118c886e83656104", "0000000000000000000000000000000000000000000000151ad982681c5191db", "000000000000000000000000000000000000000000000012c411c76c66631264", 0},
		{"00000000000000000000000000000000000000000000025c335e8cfcb013a8d9", "00000000000000000000000000000000000000000000036906ca002576ff8a0d", "000000000000000000000000000000000000000000000283953511af14ef7e1a", 0},
		{"000000000000000000000000000000000000000000000292b60e77b6650812fc", "00000000000000000000000000000000000000000000036906ca002576ff8a0d", "00000000000000000000000000000000000000000000010c1472ec394e100b21", 0},
		{"000000000000000000000000000000000000000000000298c6a0fa943ca1fd11", "00000000000000000000000000000000000000000000036906ca002576ff8a0d", "0000000000000000000000000000000000000000000000abc78bae79db5e8fe2", 0},
		{"000000000000000000000000000000000000000000000568f5083f733ab63e2f", "0000000000000000000000000000000000000000000014f5acd47daf70f22e91", "00000000000000000000000000000000000000000000095f54dd4cb1a4681e9a", 0},
		{"000000000000000000000000000000000000000000000946ba8c37dfd5d12f31", "0000000000000000000000000000000000000000000014f5acd47daf70f22e91", "000000000000000000000000000000000000000000000c2b3861243d6d04e80b", 0},
		{"000000000000000000000000000000000000000000000c6fd1d31fe7764d3823", "0000000000000000000000000000000000000000000014f5acd47daf70f22e91", "00000000000000000000000000000000000000000000065f4cdc1976f87ed3bf", 0},
		{"000000000000000000000000000000000000000000000ef402c2ef784da56ec7", "000000000000000000000000000000000000000000002b246f343a228af13c27", "00000000000000000000000000000000000000000000190c804f55810a688243", 0},
		{"00000000000000000000000000000000000000000000234221498d7b80d1be96", "000000000000000000000000000000000000000000002b246f343a228af13c27", "0000000000000000000000000000000000000000000007da07ddabebafb16c1a", 0},
		{"00000000000000000000000000000000000000000000249180ebfd20a99805d8", "000000000000000000000000000000000000000000002b246f343a228af13c27", "00000000000000000000000000000000000000000000112a5b320a1e49e6b94e", 0},
		{"000000000000000000000000000000000000000000655e1e2623daa67368cdc6", "000000000000000000000000000000000000000000b5325138b68b89cbf6b36b", "0000000000000000000000000000000000000000009544af10a657260c947656", 0},
		{"00000000000000000000000000000000000000000091994dfbc71a0334a06ca2", "000000000000000000000000000000000000000000b5325138b68b89cbf6b36b", "0000000000000000000000000000000000000000003d6f0a5e85365f83180ed5", 0},
		{"00000000000000000000000000000000000000000095a02d6577320239f51544", "000000000000000000000000000000000000000000b5325138b68b89cbf6b36b", "0000000000000000000000000000000000000000003a04e6a88bd29f8c915f38", 0},
		{"000000000000000000000000000000000000000010f3c906b3cd8242ecbfa2ee", "00000000000000000000000000000000000000002f7575d847d80bb1163b5e35", "00000000000000000000000000000000000000000ebeef13b59dc7c8e01449e7", 0},
		{"000000000000000000000000000000000000000013ccb38c1012d094efa6585b", "00000000000000000000000000000000000000002f7575d847d80bb1163b5e35", "00000000000000000000000000000000000000000f06dd6d8039b398ad0e9a44", 0},
		{"0000000000000000000000000000000000000000179de03c09590dcb79d374f2", "00000000000000000000000000000000000000002f7575d847d80bb1163b5e35", "0000000000000000000000000000000000000000154f5b2e4bbee315f58df218", 0},
		{"00000000000000000000000000000000000000004e94660e408a206644a1eaf3", "00000000000000000000000000000000000000070bbfd0fd0bfb1bfab1766ad3", "00000000000000000000000000000000000000041273924fec88ba4a15b88189", 0},
		{"00000000000000000000000000000000000000033ad5f8ebf02c3359020819ee", "00000000000000000000000000000000000000070bbfd0fd0bfb1bfab1766ad3", "000000000000000000000000000000000000000188a326301a4395043cb91a74", 0},
		{"000000000000000000000000000000000000000516b5c1fb0c7c0830a1d8b7c8", "000000000000000000000000000000000000007988e11eccca674cecab3f9a4d", "00000000000000000000000000000000000000255f3dfc74bfe6f71a5162e9df", 0},
		{"0000000000000000000000000000000000000005f6651b19ad888e9904f99109", "00000000000000000000000000000000000000070bbfd0fd0bfb1bfab1766ad3", "000000000000000000000000000000000000000233425b369cacc15a030e61e4", 0},
		{"000000000000000000000000000000000000003745daa466334fddab07ad6dc3", "000000000000000000000000000000000000007988e11eccca674cecab3f9a4d", "000000000000000000000000000000000000003a25582c50018273a9b61d8b88", 0},
		{"000000000000000000000000000000000000005c4aafa15dc141be1beceab85c", "000000000000000000000000000000000000007988e11eccca674cecab3f9a4d", "0000000000000000000000000000000000000054a1d08ccdb7bb10c703989b23", 0},
		{"00000000000000000000000000000000000001947be30be8a8c5633724a86988", "00000000000000000000000000000000000005f955625470fb84dc2c96eb5155", "00000000000000000000000000000000000003b22c88ac56d61b919f96a29b0b", 0},
		{"00000000000000000000000000000000000001b2730f6d7191c7a2cf2963631e", "00000000000000000000000000000000000005f955625470fb84dc2c96eb5155", "00000000000000000000000000000000000001a99bc48812a28417895193317c", 0},
		{"00000000000000000000000000000000000003945101821dcdcc18fac69fc50c", "00000000000000000000000000000000000005f955625470fb84dc2c96eb5155", "00000000000000000000000000000000000000d3863434f50e55fe4be10a54b3", 0},
		{"0000000000000000000000000000000016434ab80d50d9a2b38efd2074e43f9b", "0000000000000000000000000000000160978deee0fa4648ed9e77b0182e1f59", "000000000000000000000000000000009aba258a8e58cfeff1d96cb3be9cb47f", 0},
		{"00000000000000000000000000000000899a209379b27c36b15298d38adf42fe", "0000000000000000000000000000000160978deee0fa4648ed9e77b0182e1f59", "00000000000000000000000000000000cd8a66569a65acbc499af84b08e29faf", 0},
		{"00000000000000000000000000000001005fc2480c3f873dd7c5e258185b950d", "0000000000000000000000000000000160978deee0fa4648ed9e77b0182e1f59", "00000000000000000000000000000001016dc163ec0c69a8b60ec44349837c3f", 0},
		{"00000000000000000000000000001c2ab91b1e3ffff394b4f1fca8df5720cf93", "0000000000000000000000000000af153ca545a869bfbc150dfbab1f930435f1", "00000000000000000000000000001c6de2b5334aa826cefe327d4db1fdb9f488", 0},
		{"00000000000000000000000000002fbb5ec571b39c4afa23c7be9eb23722890a", "0000000000000000000000000000af153ca545a869bfbc150dfbab1f930435f1", "00000000000000000000000000004851a4db9781313444b0e1040c62243889c8", 0},
		{"0000000000000000000000000000797432b9a3007508bcf43b6c4a74ed14c882", "0000000000000000000000000000af153ca545a869bfbc150dfbab1f930435f1", "00000000000000000000000000008da8a8d5bad01f037c22687cbbf5a395ceb2", 0},
		{"00000000000000000000000000015d94d758da5dee7c4e025898323b2bfaf5df", "0000000000000000000000000002a61b3aaccff2d254d212a45072efefa20dc7", "000000000000000000000000000090d3517a0222912b985a576dbf7dee73309c", 0},
		{"000000000000000000000000000285163e074bb132f134d22294bdeff54051f9", "0000000000000000000000000002a61b3aaccff2d254d212a45072efefa20dc7", "0000000000000000000000000001d18762620dd4644114f1ba98cf8e039b3527", 0},
		{"000000000000000000000000000296158e2e5a98d7bce3f60a2aa96bbdcfa45c", "0000000000000000000000000002a61b3aaccff2d254d212a45072efefa20dc7", "000000000000000000000000000039deaf00dd82f95c74432dbc5f3aab0bfb0b", 0},
		{"000000000000000000001a507c677b153e09e64381ec686d570f98fb02464a67", "000000000000000000011b3a74a022f98ab287cb1971126f312bb389101d174f", "00000000000000000000a0511b7bc8a13b0fe38d61614471b35fd684ff709c9e", 0},
		{"00000000000000000000a1d86a796dc683648900b7ca7ed9e0b05c352a4d68b1", "0000000000000000000532f98ba82adf743412bdc68df4fc32571fca8fb1ad9b", "0000000000000000000258141be7d7a5e1481862ec8fe84a02cfcb3cdc0e0711", 0},
		{"00000000000000000000b6a79ad444a0c0671b1ef890c47ffc2913dea0025365", "000000000000000000011b3a74a022f98ab287cb1971126f312bb389101d174f", "00000000000000000000c26ed6bd3b96df8f2d402d1fc73ab6e81a3ad6c6fc40", 0},
		{"00000000000000000000f168f7d05c431203e1b61bf01bce5275821398be6b62", "000000000000000000011b3a74a022f98ab287cb1971126f312bb389101d174f", "00000000000000000000bd793c5837c37a90b57cb879e488052248ec9d4ae68f", 0},
		{"00000000000000000002cd9be54727a8ce944acf1e9714cdea41a396e464f8ea", "0000000000000000000532f98ba82adf743412bdc68df4fc32571fca8fb1ad9b", "0000000000000000000043be28f4dae4af00c1744699dd06b8e4da29f8554509", 0},
		{"000000000000000000047b19654aab3a59db7b0238336d971f8c96bb03e5a0ba", "0000000000000000000532f98ba82adf743412bdc68df4fc32571fca8fb1ad9b", "00000000000000000001aab1828aa5a982a2a6f314d6110880d5f7858d111a2c", 0},
		{"000000000000000000056cc15f0fdfa07b4b04bfc45afecd01d930d639caaf37", "0000000000000000000f471097a2c1e28348103342d73f52a7b368a102178245", "000000000000000000046d21be28b863bfaad4c66e174b1fd122d092cb4bb790", 0},
		{"00000000000000000005ae79805abaf758836444704d6879d5e30783706c9686", "0000000000000000000f471097a2c1e28348103342d73f52a7b368a102178245", "00000000000000000005c43459be13279cad1ba47a1d99782903de5afc0f9837", 0},
		{"0000000000000000000601aac2eda9e30ebc0498fbb02ac259d6372a44385c4e", "0000000000000000000f471097a2c1e28348103342d73f52a7b368a102178245", "00000000000000000000971e94393d22f3a78a1a32b516467e6a19e4179897aa", 0},
		{"000000000000000000173b9459af524e609682613236ffedbe36a44eccbfaecd", "0000000000000000009aea403cc86257493a47b011d4785bb3fdeec172e16c95", "000000000000000000330814d19e417f249f36a8f9b74d371cc6dd1da8c29130", 0},
		{"0000000000000000005ab87c4ce0ae14b28634b84d7052846c078cda85965f4f", "0000000000000000009aea403cc86257493a47b011d4785bb3fdeec172e16c95", "0000000000000000006ee50ef1a18c1bf8e1252b16a4b53bd5e1834f46e79d6c", 0},
		{"00000000000000000089ffd79c887751cc57a2bd9db3cc7e806cbef8154aa7f0", "0000000000000000009aea403cc86257493a47b011d4785bb3fdeec172e16c95", "0000000000000000002f4497a79c8d91d479afd74f00517c1a8adfc55d456a00", 0},
		{"0000000000000003868d14868fadb371ff02ea0727e1039db3c980b8008b35c6", "000000000000000ef1ca1c30d13f0e75d0654fc9c0b25836774a83a6d145a65d", "000000000000000ac6a07090d7a5dea9d83e14d980147641142301f36c5307ec", 0},
		{"0000000000000005f3add7c360c78019f1bd71da0a75b84c70e430dfb299a1a3", "000000000000000ef1ca1c30d13f0e75d0654fc9c0b25836774a83a6d145a65d", "0000000000000003ed3c26410b3952cf69776e13a07ccc6618a4fe4c42847556", 0},
		{"00000000000000098952a70960a7c78fe7e924d48dedce43ee9e6dbcbbdb95f4", "000000000000000ef1ca1c30d13f0e75d0654fc9c0b25836774a83a6d145a65d", "000000000000000c6d5bc0fdc35798e0d1090c8e055fb59a44a2e51a0ea62e95", 0},
		{"0000000000000017ed7632e0d479eaa27db8063b24c48aa53dcf5be5b50e47e6", "0000000000000077c90315a5e82fd3f2614ee8580ab24d6cf6f71879c5a1ad61", "000000000000005f5a816b9a126f8f2e26c2b3b8e126abae0ede919c43fca1e7", 0},
		{"000000000000003074438a9e94bfe2faee9ede1943fb906002a598c3c19212b3", "0000000000000077c90315a5e82fd3f2614ee8580ab24d6cf6f71879c5a1ad61", "0000000000000026bfcdf00e8ac90dcbe1bdc5e4487de5544902e6bb8fbd22b6", 0},
		{"00000000000000447ed29fd899037b711334c4d35ff394845069325becb1931a", "0000000000000077c90315a5e82fd3f2614ee8580ab24d6cf6f71879c5a1ad61", "0000000000000038af2e9e173031bad5eae90851b209c72ec61da377bb8353fc", 0},
		{"00000000000000b77fd147681fdfc372fe0d9fc308656ebab1111dd542884af9", "00000000000018de7a73426ba32c2fe958083681c717047c06fe8c993674d9cb", "00000000000015a07cfba9840861cdf7a3ac8e49003aa73ae788f1134edaed43", 0},
		{"0000000000000348af484312a97f6824af8c304de500984d17573c5500564042", "000000000000177389d552d0c927478ad3e4a82fa1f247bf7672ed21337c2991", "00000000000006804b8f91a90d1e6082c6d913dbcdf9a7ee7fd8f2f190ca11c8", 0},
		{"00000000000003d261acf86a65af0014f4e7fba6302d545d1243852a9337af12", "00000000000018de7a73426ba32c2fe958083681c717047c06fe8c993674d9cb", "00000000000006a7fa5ccd5330746237cbd97238259e88ca335f235f2f9e0b04", 0},
		{"0000000000000509aea53884538ec0861775d7b5ef907db0c236afae9a79f881", "00000000000018de7a73426ba32c2fe958083681c717047c06fe8c993674d9cb", "000000000000129b6240b5794d730510fe027a3876e357ad0dfab935af86d37e", 0},
		{"00000000000008a56008816204f89aa67f94c08fb4ef17a90dc89ab53bafd021", "000000000000177389d552d0c927478ad3e4a82fa1f247bf7672ed21337c2991", "00000000000004dc07b7322e1323fdce44193637deb36f8cf546175ce2a09298", 0},
		{"0000000000000d952cb12354b3dd190e8f72d721aea6afd3ae467e3e15c8d7ef", "000000000000177389d552d0c927478ad3e4a82fa1f247bf7672ed21337c2991", "00000000000011e42921c1ee678934d7b0e16ca63e42b7b0256ba20b38158aa0", 0},
		{"000000000022fb8e962951c7068d05db5d047ce6f30e38e5ca88cba6661a9354", "00000000031af60245ad37eec3f03a1dc31d08c51f155c696a9dbefce4584403", "00000000019dae91ced842c75343eb86fcddf21de188a627bc4aea27fac9c54d", 0},
		{"000000000040401d4286086d08a867452da70f8b4797c11df81747d85cdf9ec8", "00000000031af60245ad37eec3f03a1dc31d08c51f155c696a9dbefce4584403", "0000000000a2a501692829fb2b6d5f90852f382a2231d178d2c6d871c94ceed9", 0},
		{"000000000103078b6664f72c385390db6a59dd41cfac2f9e353ae057977bef06", "00000000031af60245ad37eec3f03a1dc31d08c51f155c696a9dbefce4584403", "0000000002be153c0f28c857f4f0ebb7172f343ff9a96cf2fd3c6405818ee7a4", 0},
		{"00000000011873c6ec61a64044724725f41e17e3ea0405ac4c342554f606ff07", "0000000002821d4f811cd47467c5d72405c137ac84e851c3a058937ac391753f", "00000000001508c2da5590c5ab9af4070d1d2dde3fe923b3921056b5de2de85a", 0},
		{"0000000001bec00c0972cc7c71bfbbb6a46807d344e1a324da16e5c7720ba5d4", "0000000002821d4f811cd47467c5d72405c137ac84e851c3a058937ac391753f", "00000000006e503ecd14ecb690319e7b271f4c05d5e590b67c0d4b652b0a713f", 0},
		{"0000000001e265a3e6078b1833b3b7eb51710ed7f9eb585b27e940abcf18ead9", "0000000002821d4f811cd47467c5d72405c137ac84e851c3a058937ac391753f", "00000000020cef933720e69779b16c949c3bcd7cbc8fe809a572097c24df9596", 0},
		{"000002a45e256282090bc6aeaaf1e622f4321acd4fce079c3123eb7832c52b01", "00001c634eb2ecc0b389a5248eeb74a207a13ea970904505e0f237e446b2655b", "000014bc1ae1a795b85e1dade7e616d04e8f2f42b5abd09696889d7477ee1dbb", 0},
		{"00000f668c318d950b49b581914226752a4b8fe116c0045afc68f5ee1faf6c70", "00001c634eb2ecc0b389a5248eeb74a207a13ea970904505e0f237e446b2655b", "00000a2e067c067f5eac89178a62ee74d4d0951915e3ddb4cf04c2067f9bd988", 0},
		{"00001543676fd264d9e0f790335c9db0f582cac7a79f7eb59b24aad95a890bda", "00001c634eb2ecc0b389a5248eeb74a207a13ea970904505e0f237e446b2655b", "00001b615b57d0963db6990bc9e918f7e3450690c2917c70189f08d45a466205", 0},
		{"004ac901b4ad0dace2c837efbae784f7661647919b5d11f694fd2694352a1845", "030f5ababfe96c57d7fc25f26d06113682ef1cc4cb1a3f086cdfeaf5f5928127", "023c3fd20f02832e3dfcacec409377d3e36558386212942c14b127412889d80a", 0},
		{"0278d3a67a0d2dce7058a2fa7ef96aae8864468fb3106a6c3a6dd0815912ce12", "030f5ababfe96c57d7fc25f26d06113682ef1cc4cb1a3f086cdfeaf5f5928127", "0006c33f179667ce775924b8b71c56b926541dbdceff2bd463f7018f8c060413", 0},
		{"02fec3a86c904df34ff48b889d3f2271eb270f8ab63eeecb194971de4218da79", "030f5ababfe96c57d7fc25f26d06113682ef1cc4cb1a3f086cdfeaf5f5928127", "02f75ec2d3e5387c9f83d5abc7ced84da4e127d8bfb4a17d67a1565de292ef56", 0},
		{"062de676638605d0f9deb955929d1939ecf377f1f25b5f3886285aa03710b62c", "4554f111e81655a9d42c332c0d436a4e39f53cc999a5ab111b68d931a1124519", "1d86e0291807c9c84d0c7b2f77e6f6f362d7beefbe174e2007c741633195ec45", 0},
		{"15d2e2bf94051026b54336f6ac15d6a9c1e171f84d5ded7d4eab86861edbfb5a", "4554f111e81655a9d42c332c0d436a4e39f53cc999a5ab111b68d931a1124519", "03345d5426f3b38bfaa2dc1dc4ffb4906724b32a4882c728022579b4fe6997a4", 0},
		{"36062bc132b5ecbfdbcd75c1822115b2b93977bb2e5bc7fc8b8a51abe1d4d081", "4554f111e81655a9d42c332c0d436a4e39f53cc999a5ab111b68d931a1124519", "25d9fa60c4dd3d67f9573bc2ba866f6b2b295df1294a47006a5406e7a476fd24", 0},
		{"c0ee5c43aadb01d95b8e7c300dfedcf2487fe21f02679c16fb44a1b8fe7f8d36", "e5a5fe281aed3487822ffd2442a2484a244a168c0b59215ac3c686c9b650ec7b", "e32701cc43a21e8879595eda986f1b277ebfa9ca7ca3cdb39b46a361c9588fd0", 0},
		{"c7dda628a3d4ee89d5e85e52489e2a0bd4fa7978667fe5a7d8635a3b39e32d52", "e5a5fe281aed3487822ffd2442a2484a244a168c0b59215ac3c686c9b650ec7b", "1e2e6e2076aa8a9b7d957d43feab45a7e464b2de9ea93280c94fdaf3759eb93c", 0},
		{"cb1091d01c0a0019534fe60409f37b0ab199957e307dcd34375af0eb28027617", "e5a5fe281aed3487822ffd2442a2484a244a168c0b59215ac3c686c9b650ec7b", "196bc84da4d52061821dbe6e76f91f616aed7de5f9ad225bc607602a69dbbcb7", 0},
	};
	for (unsigned int i = 0; i < ARRAY_LENGTH(cases); i++) {
		TernaryCase &tc = cases[i];
		Uint256 x(tc.x);
		x.reciprocal(Uint256(tc.y));
		assert(x == Uint256(tc.z));
		numTestCases++;
	}
}


static void testReplaceAndSwap() {
	BinaryCase cases[] = {
		{"0000000000000000000000000000000000000000000000000000000000000000", "0000000000000000000000000000000000000000000000000000000000000000"},
		{"0000000000000000000000000000000000000000000000000000000000000000", "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"},
		{"ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", "0000000000000000000000000000000000000000000000000000000000000000"},
		{"ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff", "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"},
		{"0000000000000100000000000000000000000000000000000000000000000000", "0000000000000000000000000000000000000000000000000000000000000008"},
		{"12041f8a08b389ea4e0e06af09e0929efb8b1b86dec57d6fdeb3d9d115f6248b", "fa84d9fc056efb0fb361e3e1be0ab792eb1e264f64bc98e430280984076f48e9"},
		{"eb3031d2e93d9d6a5acc06749cb81e22aa356a2cc993e9a6248ebb894b6433eb", "38c412bd1e74230eb2239752396c184138b71a2098b36982a7abe540f87f9daa"},
		{"0e1aa5a5161df93a4137bcdcd82e4f0cd0915cb90c5cd06470240f6a2180b8a1", "7b0ccf9e16381bc87ff7aabd209c9245a78eb029241fe3c38723ba8e7e5bb63e"},
		{"0754d05eae2a7f834400dc2caf9de9703bf68de8239e0250e09922fb4948661e", "29f0dba4ed66f5013a1c73b713ceb46b243769e7cdf6ba535e7cfaec1708f998"},
		{"16e6f9a0e9fe2bc49e0042bd66e6977b96beecd1acfc9d8a59ddb56e8b2da33e", "8fc4e7fed2002227c5c8816195d2fe95968968085522e147b64914d365a763ec"},
		{"4f82285de18823579d43e3570f7036a2dcc9ded10556ddd21d98b5ab54016d99", "67c27d521e4ed8c5614d954c270e342ee6c1cf39147411f6d096cba0a9c401bb"},
		{"56bbc0162e1402a5f93ab30db97508716c40de99bb5aeb5445b0bfe8c20499e7", "a9e5b2315a2e457a9bdbbb2fddcbee9bcd35e6dc9ab782a28828302d3909e1ef"},
		{"cadeb692ca96963bc61bc9771164fae51c86dd1635840ff60d4358cf67323dad", "413119d653c67245906e421f502a80c12157155ece0bf9da48f6a3bf961c7a91"},
	};
	for (unsigned int i = 0; i < ARRAY_LENGTH(cases); i++) {
		BinaryCase &tc = cases[i];
		Uint256 a(tc.x);
		Uint256 b(tc.y);
		Uint256 x(tc.x);
		Uint256 y(tc.y);
		x.replace(b, 0);
		assert(x == a);
		x.replace(b, 1);
		assert(x == b);
		y.replace(a, 1);
		assert(y == a);
		x.swap(y, 0);
		assert(x == b && y == a);
		x.swap(y, 1);
		assert(x == a && y == b);
		numTestCases++;
	}
}


static void testConstructorBytes() {
	uint8_t b[32] = {
		0x03, 0x4D, 0x03, 0x33,
		0x2D, 0xCE, 0x3A, 0x5F,
		0xA5, 0xCA, 0x65, 0x3B,
		0x54, 0x33, 0x5E, 0x14,
		0x81, 0x38, 0xB3, 0xA1,
		0x3C, 0x27, 0x95, 0xA3,
		0x48, 0xE6, 0x9E, 0xFE,
		0xA7, 0xCA, 0xC5, 0x16,
	};
	Uint256 x(b);
	assert(x.value[0] == UINT32_C(0xA7CAC516));
	assert(x.value[1] == UINT32_C(0x48E69EFE));
	assert(x.value[4] == UINT32_C(0x54335E14));
	assert(x.value[7] == UINT32_C(0x034D0333));
}


static void testGetBigEndianByte() {
	Uint256 x("292180db102e1f14bb371f9016e785d2c83b59aa29497ba9e595025d9373a038");
	uint8_t b[32];
	x.getBigEndianBytes(b);
	assert(b[ 0] == 0x29);
	assert(b[ 1] == 0x21);
	assert(b[ 2] == 0x80);
	assert(b[ 4] == 0x10);
	assert(b[ 7] == 0x14);
	assert(b[22] == 0x7b);
	assert(b[29] == 0x73);
	assert(b[31] == 0x38);
	numTestCases++;
}


int main(int argc, char **argv) {
	testComparison();
	testAdd();
	testSubtract();
	testShiftLeft1();
	testShiftRight1();
	testReciprocal();
	testReplaceAndSwap();
	testConstructorBytes();
	testGetBigEndianByte();
	printf("All %d test cases passed\n", numTestCases);
	return 0;
}
