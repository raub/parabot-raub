/*-
 * Copyright 2013 Garrett D'Amore <garrett@damore.org>
 * Copyright 2011 Nexenta Systems, Inc.  All rights reserved.
 * Copyright (c) 2002-2004 Tim J. Robbins
 * All rights reserved.
 *
 * Copyright (c) 2011 The FreeBSD Foundation
 * All rights reserved.
 * Portions of this software were developed by David Chisnall
 * under sponsorship from the FreeBSD Foundation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *	 notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *	 notice, this list of conditions and the following disclaimer in the
 *	 documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

struct utf8_tm
{
	int ra, rz, rdiff;
};

static struct utf8_tm mapupper[] =
{
	{0x0061, 0x007A, 0x0020}, // Latin letters
	{0x00E0, 0x00FE, 0x0020},
	{0x00FF, 0x00FF, 0x0187},
	{0x0101, 0x0101, 0x0001},
	{0x0103, 0x0103, 0x0001},
	{0x0105, 0x0105, 0x0001},
	{0x0107, 0x0107, 0x0001},
	{0x0109, 0x0109, 0x0001},
	{0x010B, 0x010B, 0x0001},
	{0x010D, 0x010D, 0x0001},
	{0x010F, 0x010F, 0x0001},
	{0x0111, 0x0111, 0x0001},
	{0x0113, 0x0113, 0x0001},
	{0x0115, 0x0115, 0x0001},
	{0x0117, 0x0117, 0x0001},
	{0x0119, 0x0119, 0x0001},
	{0x011B, 0x011B, 0x0001},
	{0x011D, 0x011D, 0x0001},
	{0x011F, 0x011F, 0x0001},
	{0x0121, 0x0121, 0x0001},
	{0x0123, 0x0123, 0x0001},
	{0x0125, 0x0125, 0x0001},
	{0x0127, 0x0127, 0x0001},
	{0x0129, 0x0129, 0x0001},
	{0x012B, 0x012B, 0x0001},
	{0x012D, 0x012D, 0x0001},
	{0x012F, 0x012F, 0x0001},
	{0x0131, 0x0131, 0x0178},
	{0x0133, 0x0133, 0x0001},
	{0x0135, 0x0135, 0x0001},
	{0x0137, 0x0137, 0x0001},
	{0x013A, 0x013A, 0x0001},
	{0x013C, 0x013C, 0x0001},
	{0x013E, 0x013E, 0x0001},
	{0x0140, 0x0140, 0x0001},
	{0x0142, 0x0142, 0x0001},
	{0x0144, 0x0144, 0x0001},
	{0x0146, 0x0146, 0x0001},
	{0x0148, 0x0148, 0x0001},
	{0x014B, 0x014B, 0x0001},
	{0x014D, 0x014D, 0x0001},
	{0x014F, 0x014F, 0x0001},
	{0x0151, 0x0151, 0x0001},
	{0x0153, 0x0153, 0x0001},
	{0x0155, 0x0155, 0x0001},
	{0x0157, 0x0157, 0x0001},
	{0x0159, 0x0159, 0x0001},
	{0x015B, 0x015B, 0x0001},
	{0x015D, 0x015D, 0x0001},
	{0x015F, 0x015F, 0x0001},
	{0x0161, 0x0161, 0x0001},
	{0x0163, 0x0163, 0x0001},
	{0x0165, 0x0165, 0x0001},
	{0x0167, 0x0167, 0x0001},
	{0x0169, 0x0169, 0x0001},
	{0x016B, 0x016B, 0x0001},
	{0x016D, 0x016D, 0x0001},
	{0x016F, 0x016F, 0x0001},
	{0x0171, 0x0171, 0x0001},
	{0x0173, 0x0173, 0x0001},
	{0x0175, 0x0175, 0x0001},
	{0x0177, 0x0177, 0x0001},
	{0x017A, 0x017A, 0x0001},
	{0x017C, 0x017C, 0x0001},
	{0x017E, 0x017E, 0x0001},
	{0x0183, 0x0183, 0x0001},
	{0x0185, 0x0185, 0x0001},
	{0x0188, 0x0188, 0x0001},
	{0x018C, 0x018C, 0x0001},
	{0x0192, 0x0192, 0x0001},
	{0x0199, 0x0199, 0x0001},
	{0x01A1, 0x01A1, 0x0001},
	{0x01A3, 0x01A3, 0x0001},
	{0x01A5, 0x01A5, 0x0001},
	{0x01A8, 0x01A8, 0x0001},
	{0x01AD, 0x01AD, 0x0001},
	{0x01B0, 0x01B0, 0x0001},
	{0x01B4, 0x01B4, 0x0001},
	{0x01B6, 0x01B6, 0x0001},
	{0x01B9, 0x01B9, 0x0001},
	{0x01BD, 0x01BD, 0x0001},
	{0x01C6, 0x01C6, 0x0002},
	{0x01C9, 0x01C9, 0x000E},
	{0x01CC, 0x01CC, 0x0006},
	{0x01CE, 0x01CE, 0x0001},
	{0x01D0, 0x01D0, 0x0001},
	{0x01D2, 0x01D2, 0x0001},
	{0x01D4, 0x01D4, 0x0001},
	{0x01D6, 0x01D6, 0x0001},
	{0x01D8, 0x01D8, 0x0001},
	{0x01DA, 0x01DA, 0x0001},
	{0x01DC, 0x01DC, 0x0001},
	{0x01DF, 0x01DF, 0x0001},
	{0x01E1, 0x01E1, 0x0001},
	{0x01E3, 0x01E3, 0x0001},
	{0x01E5, 0x01E5, 0x0001},
	{0x01E7, 0x01E7, 0x0001},
	{0x01E9, 0x01E9, 0x0001},
	{0x01EB, 0x01EB, 0x0001},
	{0x01ED, 0x01ED, 0x0001},
	{0x01EF, 0x01EF, 0x0001},
	{0x01F3, 0x01F3, 0x0001},
	{0x01F5, 0x01F5, 0x0001},
	{0x01FB, 0x01FB, 0x0001},
	{0x01FD, 0x01FD, 0x0001},
	{0x01FF, 0x01FF, 0x0001},
	{0x0201, 0x0201, 0x0001},
	{0x0203, 0x0203, 0x0001},
	{0x0205, 0x0205, 0x0001},
	{0x0207, 0x0207, 0x0001},
	{0x0209, 0x0209, 0x0001},
	{0x020B, 0x020B, 0x0001},
	{0x020D, 0x020D, 0x0001},
	{0x020F, 0x020F, 0x0001},
	{0x0211, 0x0211, 0x0001},
	{0x0213, 0x0213, 0x0001},
	{0x0215, 0x0215, 0x0001},
	{0x0217, 0x0217, 0x0001},
	{0x0253, 0x0254, 0x03D2},
	{0x0257, 0x0257, 0x03DD},
	{0x0258, 0x0259, 0x03D6},
	{0x025B, 0x025B, 0x03CB},
	{0x0260, 0x0260, 0x03F3},
	{0x0263, 0x0263, 0x03F7},
	{0x0268, 0x0269, 0x03FF},
	{0x026F, 0x026F, 0x03F3},
	{0x0272, 0x0272, 0x03EF},
	{0x0275, 0x0275, 0x03EA},
	{0x0283, 0x0283, 0x032A},
	{0x0288, 0x0288, 0x0326},
	{0x028A, 0x028A, 0x033B},
	{0x028B, 0x028B, 0x0339},
	{0x0292, 0x0292, 0x0325},
	{0x03AC, 0x03AC, 0x002F}, // Greek letters
	{0x03AD, 0x03AD, 0x0025},
	{0x03AE, 0x03AE, 0x0027},
	{0x03AF, 0x03AF, 0x0025},
	{0x03B1, 0x03BF, 0x0020},
	{0x03C0, 0x03CC, 0x0060},
	{0x03CD, 0x03CE, 0x0040},
	{0x03E3, 0x03E3, 0x0001}, // Coptic letters
	{0x03E5, 0x03E5, 0x0001},
	{0x03E7, 0x03E7, 0x0001},
	{0x03E9, 0x03E9, 0x0001},
	{0x03EB, 0x03EB, 0x0001},
	{0x03ED, 0x03ED, 0x0001},
	{0x03EF, 0x03EF, 0x0001},
	{0x0430, 0x043F, 0x0020}, // Cyrillic letters
	{0x0440, 0x044F, 0x0060},
	{0x0451, 0x045F, 0x0050},
	{0x0461, 0x0461, 0x0001},
	{0x0463, 0x0463, 0x0001},
	{0x0465, 0x0465, 0x0001},
	{0x0467, 0x0467, 0x0001},
	{0x0469, 0x0469, 0x0001},
	{0x046B, 0x046B, 0x0001},
	{0x046D, 0x046D, 0x0001},
	{0x046F, 0x046F, 0x0001},
	{0x0471, 0x0471, 0x0001},
	{0x0473, 0x0473, 0x0001},
	{0x0475, 0x0475, 0x0001},
	{0x0477, 0x0477, 0x0001},
	{0x0479, 0x0479, 0x0001},
	{0x047B, 0x047B, 0x0001},
	{0x047D, 0x047D, 0x0001},
	{0x047F, 0x047F, 0x0001},
	{0x0481, 0x0481, 0x0001},
	{0x0491, 0x0491, 0x0001},
	{0x0493, 0x0493, 0x0001},
	{0x0495, 0x0495, 0x0001},
	{0x0497, 0x0497, 0x0001},
	{0x0499, 0x0499, 0x0001},
	{0x049B, 0x049B, 0x0001},
	{0x049D, 0x049D, 0x0001},
	{0x049F, 0x049F, 0x0001},
	{0x04A1, 0x04A1, 0x0001},
	{0x04A3, 0x04A3, 0x0001},
	{0x04A5, 0x04A5, 0x0001},
	{0x04A7, 0x04A7, 0x0001},
	{0x04A9, 0x04A9, 0x0001},
	{0x04AB, 0x04AB, 0x0001},
	{0x04AD, 0x04AD, 0x0001},
	{0x04AF, 0x04AF, 0x0001},
	{0x04B1, 0x04B1, 0x0001},
	{0x04B3, 0x04B3, 0x0001},
	{0x04B5, 0x04B5, 0x0001},
	{0x04B7, 0x04B7, 0x0001},
	{0x04B9, 0x04B9, 0x0001},
	{0x04BB, 0x04BB, 0x0001},
	{0x04BD, 0x04BD, 0x0001},
	{0x04BF, 0x04BF, 0x0001},
	{0x04C2, 0x04C2, 0x0001},
	{0x04C4, 0x04C4, 0x0001},
	{0x04C8, 0x04C8, 0x0001},
	{0x04CC, 0x04CC, 0x0001},
	{0x04D1, 0x04D1, 0x0001},
	{0x04D3, 0x04D3, 0x0001},
	{0x04D5, 0x04D5, 0x0001},
	{0x04D7, 0x04D7, 0x0001},
	{0x04D9, 0x04D9, 0x0001},
	{0x04DB, 0x04DB, 0x0001},
	{0x04DD, 0x04DD, 0x0001},
	{0x04DF, 0x04DF, 0x0001},
	{0x04E1, 0x04E1, 0x0001},
	{0x04E3, 0x04E3, 0x0001},
	{0x04E5, 0x04E5, 0x0001},
	{0x04E7, 0x04E7, 0x0001},
	{0x04E9, 0x04E9, 0x0001},
	{0x04EB, 0x04EB, 0x0001},
	{0x04EF, 0x04EF, 0x0001},
	{0x04F1, 0x04F1, 0x0001},
	{0x04F3, 0x04F3, 0x0001},
	{0x04F5, 0x04F5, 0x0001},
	{0x04F9, 0x04F9, 0x0001},
	{0x0561, 0x0586, 0x0030}, // Armenian letters
	{0x10D0, 0x10F5, 0x0030}, // Georgian letters
	{0x1E01, 0x1E01, 0x0001}, // Latin letters
	{0x1E03, 0x1E03, 0x0001},
	{0x1E05, 0x1E05, 0x0001},
	{0x1E07, 0x1E07, 0x0001},
	{0x1E09, 0x1E09, 0x0001},
	{0x1E0B, 0x1E0B, 0x0001},
	{0x1E0D, 0x1E0D, 0x0001},
	{0x1E0F, 0x1E0F, 0x0001},
	{0x1E11, 0x1E11, 0x0001},
	{0x1E13, 0x1E13, 0x0001},
	{0x1E15, 0x1E15, 0x0001},
	{0x1E17, 0x1E17, 0x0001},
	{0x1E19, 0x1E19, 0x0001},
	{0x1E1B, 0x1E1B, 0x0001},
	{0x1E1D, 0x1E1D, 0x0001},
	{0x1E1F, 0x1E1F, 0x0001},
	{0x1E21, 0x1E21, 0x0001},
	{0x1E23, 0x1E23, 0x0001},
	{0x1E25, 0x1E25, 0x0001},
	{0x1E27, 0x1E27, 0x0001},
	{0x1E29, 0x1E29, 0x0001},
	{0x1E2B, 0x1E2B, 0x0001},
	{0x1E2D, 0x1E2D, 0x0001},
	{0x1E2F, 0x1E2F, 0x0001},
	{0x1E31, 0x1E31, 0x0001},
	{0x1E33, 0x1E33, 0x0001},
	{0x1E35, 0x1E35, 0x0001},
	{0x1E37, 0x1E37, 0x0001},
	{0x1E39, 0x1E39, 0x0001},
	{0x1E3B, 0x1E3B, 0x0001},
	{0x1E3D, 0x1E3D, 0x0001},
	{0x1E3F, 0x1E3F, 0x0001},
	{0x1E41, 0x1E41, 0x0001},
	{0x1E43, 0x1E43, 0x0001},
	{0x1E45, 0x1E45, 0x0001},
	{0x1E47, 0x1E47, 0x0001},
	{0x1E49, 0x1E49, 0x0001},
	{0x1E4B, 0x1E4B, 0x0001},
	{0x1E4D, 0x1E4D, 0x0001},
	{0x1E4F, 0x1E4F, 0x0001},
	{0x1E51, 0x1E51, 0x0001},
	{0x1E53, 0x1E53, 0x0001},
	{0x1E55, 0x1E55, 0x0001},
	{0x1E57, 0x1E57, 0x0001},
	{0x1E59, 0x1E59, 0x0001},
	{0x1E5B, 0x1E5B, 0x0001},
	{0x1E5D, 0x1E5D, 0x0001},
	{0x1E5F, 0x1E5F, 0x0001},
	{0x1E61, 0x1E61, 0x0001},
	{0x1E63, 0x1E63, 0x0001},
	{0x1E65, 0x1E65, 0x0001},
	{0x1E67, 0x1E67, 0x0001},
	{0x1E69, 0x1E69, 0x0001},
	{0x1E6B, 0x1E6B, 0x0001},
	{0x1E6D, 0x1E6D, 0x0001},
	{0x1E6F, 0x1E6F, 0x0001},
	{0x1E71, 0x1E71, 0x0001},
	{0x1E73, 0x1E73, 0x0001},
	{0x1E75, 0x1E75, 0x0001},
	{0x1E77, 0x1E77, 0x0001},
	{0x1E79, 0x1E79, 0x0001},
	{0x1E7B, 0x1E7B, 0x0001},
	{0x1E7D, 0x1E7D, 0x0001},
	{0x1E7F, 0x1E7F, 0x0001},
	{0x1E81, 0x1E81, 0x0001},
	{0x1E83, 0x1E83, 0x0001},
	{0x1E85, 0x1E85, 0x0001},
	{0x1E87, 0x1E87, 0x0001},
	{0x1E89, 0x1E89, 0x0001},
	{0x1E8B, 0x1E8B, 0x0001},
	{0x1E8D, 0x1E8D, 0x0001},
	{0x1E8F, 0x1E8F, 0x0001},
	{0x1E91, 0x1E91, 0x0001},
	{0x1E93, 0x1E93, 0x0001},
	{0x1E95, 0x1E95, 0x0001},
	{0x1EA1, 0x1EA1, 0x0001},
	{0x1EA3, 0x1EA3, 0x0001},
	{0x1EA5, 0x1EA5, 0x0001},
	{0x1EA7, 0x1EA7, 0x0001},
	{0x1EA9, 0x1EA9, 0x0001},
	{0x1EAB, 0x1EAB, 0x0001},
	{0x1EAD, 0x1EAD, 0x0001},
	{0x1EAF, 0x1EAF, 0x0001},
	{0x1EB1, 0x1EB1, 0x0001},
	{0x1EB3, 0x1EB3, 0x0001},
	{0x1EB5, 0x1EB5, 0x0001},
	{0x1EB7, 0x1EB7, 0x0001},
	{0x1EB9, 0x1EB9, 0x0001},
	{0x1EBB, 0x1EBB, 0x0001},
	{0x1EBD, 0x1EBD, 0x0001},
	{0x1EBF, 0x1EBF, 0x0001},
	{0x1EC1, 0x1EC1, 0x0001},
	{0x1EC3, 0x1EC3, 0x0001},
	{0x1EC5, 0x1EC5, 0x0001},
	{0x1EC7, 0x1EC7, 0x0001},
	{0x1EC9, 0x1EC9, 0x0001},
	{0x1ECB, 0x1ECB, 0x0001},
	{0x1ECD, 0x1ECD, 0x0001},
	{0x1ECF, 0x1ECF, 0x0001},
	{0x1ED1, 0x1ED1, 0x0001},
	{0x1ED3, 0x1ED3, 0x0001},
	{0x1ED5, 0x1ED5, 0x0001},
	{0x1ED7, 0x1ED7, 0x0001},
	{0x1ED9, 0x1ED9, 0x0001},
	{0x1EDB, 0x1EDB, 0x0001},
	{0x1EDD, 0x1EDD, 0x0001},
	{0x1EDF, 0x1EDF, 0x0001},
	{0x1EE1, 0x1EE1, 0x0001},
	{0x1EE3, 0x1EE3, 0x0001},
	{0x1EE5, 0x1EE5, 0x0001},
	{0x1EE7, 0x1EE7, 0x0001},
	{0x1EE9, 0x1EE9, 0x0001},
	{0x1EEB, 0x1EEB, 0x0001},
	{0x1EED, 0x1EED, 0x0001},
	{0x1EEF, 0x1EEF, 0x0001},
	{0x1EF1, 0x1EF1, 0x0001},
	{0x1EF3, 0x1EF3, 0x0001},
	{0x1EF5, 0x1EF5, 0x0001},
	{0x1EF7, 0x1EF7, 0x0001},
	{0x1EF9, 0x1EF9, 0x0001},
	{0x1F00, 0x1F07, 0x0008}, // Greek letters
	{0x1F10, 0x1F15, 0x0008},
	{0x1F20, 0x1F27, 0x0008},
	{0x1F30, 0x1F37, 0x0008},
	{0x1F40, 0x1F45, 0x0008},
	{0x1F51, 0x1F51, 0x0008},
	{0x1F53, 0x1F53, 0x0008},
	{0x1F55, 0x1F55, 0x0008},
	{0x1F57, 0x1F57, 0x0008},
	{0x1F60, 0x1F67, 0x0008},
	{0x1F80, 0x1F87, 0x0008},
	{0x1F90, 0x1F97, 0x0008},
	{0x1FA0, 0x1FA7, 0x0008},
	{0x1FB0, 0x1FB1, 0x0008},
	{0x1FD0, 0x1FD1, 0x0008},
	{0x1FE0, 0x1FE1, 0x0008},
	{0x24D0, 0x24E9, 0x0066}, // Circled latin letters
	{0xFF41, 0xFF5A, 0x0060}  // Fullwidth latin letters
};

static size_t UTF8_mbtowc(int *pwc, const char *s) {
	int ch, i, mask, want, lbound, wch;

	if (s == NULL) {
		s = "";
		want = 1;
		pwc = NULL;
	}

	/*
	 * Determine the number of octets that make up this character
	 * from the first octet, and a mask that extracts the
	 * interesting bits of the first octet. We already know
	 * the character is at least two bytes long.
	 *
	 * We also specify a lower bound for the character code to
	 * detect redundant, non-"shortest form" encodings. For
	 * example, the sequence C0 80 is _not_ a legal representation
	 * of the null character. This enforces a 1-to-1 mapping
	 * between character codes and their multibyte representations.
	 */
	ch = (unsigned char)*s;
	if ((ch & 0x80) == 0) {
		/* Fast path for plain ASCII characters. */
		if (pwc != NULL)
			*pwc = ch;
		return (ch != '\0' ? 1 : 0);
	}
	if ((ch & 0xe0) == 0xc0) {
		mask = 0x1f;
		want = 2;
		lbound = 0x80;
	} else if ((ch & 0xf0) == 0xe0) {
		mask = 0x0f;
		want = 3;
		lbound = 0x800;
	} else if ((ch & 0xf8) == 0xf0) {
		mask = 0x07;
		want = 4;
		lbound = 0x10000;
	} else {
		/*
		 * Malformed input; input is not UTF-8.
		 */
		if (pwc != NULL)
			*pwc = '?';
		return 1;
	}

	/*
	 * Decode the octet sequence representing the character in chunks
	 * of 6 bits, most significant first.
	 */
	wch = (unsigned char)*s++ & mask;

	for (i = 1; i < want; i++) {
		if ((*s & 0xc0) != 0x80) {
			/*
			 * Malformed input; bad characters in the middle
			 * of a character.
			 */
			if (pwc != NULL)
				*pwc = '?';
			return 1;
	}
		wch <<= 6;
		wch |= *s++ & 0x3f;
	}
	if (wch < lbound) {
		/*
		 * Malformed input; redundant encoding.
		 */
		if (pwc != NULL)
			*pwc = '?';
		return 1;
	}
	if ((wch >= 0xd800 && wch <= 0xdfff) || wch > 0x10ffff) {
		/*
		 * Malformed input; invalid code points.
		 */
		if (pwc != NULL)
			*pwc = '?';
		return 1;
	}
	if (pwc != NULL)
		*pwc = wch;
	return (wch == L'\0' ? 0 : want);
}

static size_t UTF8_wctomb(char *s, int wc) {
	unsigned char lead;
	int i, len;

	if (s == NULL)
		/* Reset to initial shift state (no-op) */
		return (1);

	/*
	 * Determine the number of octets needed to represent this character.
	 * We always output the shortest sequence possible. Also specify the
	 * first few bits of the first octet, which contains the information
	 * about the sequence length.
	 */
	if ((wc & ~0x7f) == 0) {
		/* Fast path for plain ASCII characters. */
		*s = (char)wc;
		return (1);
	} else if ((wc & ~0x7ff) == 0) {
		lead = 0xc0;
		len = 2;
	} else if ((wc & ~0xffff) == 0) {
		if (wc >= 0xd800 && wc <= 0xdfff) {
			*s = '?';
			return 1;
	}
		lead = 0xe0;
		len = 3;
	} else if (wc >= 0 && wc <= 0x10ffff) {
		lead = 0xf0;
		len = 4;
	} else {
		*s = '?';
		return 1;
	}

	/*
	 * Output the octets representing the character in chunks
	 * of 6 bits, least significant last. The first octet is
	 * a special case because it contains the sequence length
	 * information.
	 */
	for(i = len - 1; i > 0; i--)
	{
		s[i] = (wc & 0x3f) | 0x80;
		wc >>= 6;
	}
	*s = (wc & 0xff) | lead;

	return len;
}

static int UTF8_toupper(int wc) {
	for(int i = 0; i < sizeof(mapupper)/sizeof(struct utf8_tm); i++)
		if(mapupper[i].ra <= wc && wc <= mapupper[i].rz)
			return wc ^ mapupper[i].rdiff;
	return wc;
}

static void UTF8_strupr(char *s) {
	int wc;
	size_t len;

	while(*s)
	{
		UTF8_mbtowc(&wc, s);
		wc = UTF8_toupper(wc);
		len = UTF8_wctomb(s, wc);
		s += len;
		  }
}
