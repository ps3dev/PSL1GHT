#ifndef __EXPORTS_H__
#define __EXPORTS_H__


EXPORT(cellSslInit, 0xfb02c9d2);
EXPORT(cellSslEnd, 0x1650aea4);
EXPORT(cellSslCertificateLoader, 0x571afaca);
EXPORT(cellSslCertGetSerialNumber, 0x7b689ebc);
EXPORT(cellSslCertGetPublicKey, 0xf8206492);
EXPORT(cellSslCertGetRsaPublicKeyExponent, 0x033c4905);
EXPORT(cellSslCertGetRsaPublicKeyModulus, 0x8e505175);

EXPORT(cellSslCertGetNotAfter, 0x218b64da);
EXPORT(cellSslCertGetNotBefore, 0x31d9ba8d);
EXPORT(cellSslCertGetSubjectName, 0x32c61bdf);
EXPORT(cellSslCertGetIssuerName, 0xae6eb491);
EXPORT(cellSslCertGetNameEntryCount, 0x766d3ca1);
EXPORT(cellSslCertGetNameEntryInfo, 0x006c4900);
EXPORT(cellSslCertGetMd5Fingerprint, 0x5e9253ca);


#endif

