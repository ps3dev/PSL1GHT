#ifndef __SSL_H__
#define __SSL_H__


#define SSL_CIPHER_UNKNOWN                         (-1)
#define SSL_CIPHER_RSA_WITH_3DES_EDE_CBC_SHA       (0)
#define SSL_CIPHER_RSA_WITH_RC4_128_SHA            (1)
#define SSL_CIPHER_RSA_WITH_RC4_128_MD5            (2)
#define SSL_CIPHER_RSA_WITH_DES_CBC_SHA            (3)
#define SSL_CIPHER_RSA_EXPORT1024_WITH_DES_CBC_SHA (4)
#define SSL_CIPHER_RSA_EXPORT1024_WITH_RC4_56_SHA  (5)
#define SSL_CIPHER_RSA_EXPORT_WITH_DES40_CBC_SHA   (6)
#define SSL_CIPHER_RSA_EXPORT_WITH_RC2_CBC_40_MD5  (7)
#define SSL_CIPHER_RSA_EXPORT_WITH_RC4_40_MD5      (8)

#define SSL_VERSION_UNKNOWN (-1)
#define SSL_VERSION_TLS1    (0)
#define SSL_VERSION_SSL3    (1)

/* Max size for a certificate's MD5 Fingerprint */
#define SSL_MD5_FINGERPRINT_MAX_SIZE   (20)



/* SSL certificate loader flags */
#define SSL_LOAD_CERT_ALL                (SSL_LOAD_CERT_SCE |   \
                                          SSL_LOAD_CERT_NORMAL)

#define SSL_LOAD_CERT_SCE                (SSL_LOAD_CERT_SCE01 | \
                                          SSL_LOAD_CERT_SCE02 | \
                                          SSL_LOAD_CERT_SCE03 | \
                                          SSL_LOAD_CERT_SCE04 | \
                                          SSL_LOAD_CERT_SCE05)
#define SSL_LOAD_CERT_NORMAL             (SSL_LOAD_CERT_BALTIMORE_CT      | \
                                          SSL_LOAD_CERT_CLASS3_PCA_G2_V2  | \
                                          SSL_LOAD_CERT_CLASS3_PCA_G3_V2  | \
                                          SSL_LOAD_CERT_CLASS3_PCA_SS_V4  | \
                                          SSL_LOAD_CERT_ENTRUST_NET_SS_CA | \
                                          SSL_LOAD_CERT_EQUIFAX_SEC_CA    | \
                                          SSL_LOAD_CERT_EQUIFAX_SEC_EBUS1 | \
                                          SSL_LOAD_CERT_GEOTRUST_GCA      | \
                                          SSL_LOAD_CERT_GLOBALSIGN_RCA    | \
                                          SSL_LOAD_CERT_GTE_CT_G_RCA      | \
                                          SSL_LOAD_CERT_RSA1024_V1        | \
                                          SSL_LOAD_CERT_RSA2048_V3        | \
                                          SSL_LOAD_CERT_THAWTE_PREM_SCA   | \
                                          SSL_LOAD_CERT_THAWTE_SCA        | \
                                          SSL_LOAD_CERT_VALICERT_CLASS2   | \
                                          SSL_LOAD_CERT_AAA_CERT_SERVICES | \
                                          SSL_LOAD_CERT_ADDTRUST_EXT_CA   | \
                                          SSL_LOAD_CERT_UTN_USERFIRST_HW  | \
                                          SSL_LOAD_CERT_EQUIFAX_SEC_G_EBUS1 | \
                                          SSL_LOAD_CERT_CLASS3_PCA_G5     | \
                                          SSL_LOAD_CERT_DIGICERT_HA_EV_RCA| \
                                          SSL_LOAD_CERT_DIGICERT_A_ID_RCA | \
                                          SSL_LOAD_CERT_DIGICERT_GLOBAL_RCA | \
                                          SSL_LOAD_CERT_CT_G_ROOT           | \
                                          SSL_LOAD_CERT_ENTRUST_CA2048      | \
                                          SSL_LOAD_CERT_ENTRUST_RCA         | \
                                          SSL_LOAD_CERT_GODADDY_CLASS2_CA   | \
                                          SSL_LOAD_CERT_STARFIELD_CLASS2_CA | \
                                          SSL_LOAD_CERT_STARFIELD_S_RC      | \
                                          SSL_LOAD_CERT_THATE_PRIMARY_RCA   | \
                                          SSL_LOAD_CERT_VALICERT_CLASS1_VA  | \
                                          SSL_LOAD_CERT_SECOM_RCA1          | \
                                          SSL_LOAD_CERT_SECURE_TRUST_CA     | \
                                          SSL_LOAD_CERT_SHA256)

#define SSL_LOAD_CERT_SHA256            (SSL_LOAD_CERT_GLOBALSIGN_RCA_R2   | \
                                         SSL_LOAD_CERT_ENTRUST_RCA_G2      | \
                                         SSL_LOAD_CERT_VERISIGN_U_RCA      | \
                                         SSL_LOAD_CERT_GODADDY_C2_CA_G2    | \
                                         SSL_LOAD_CERT_STARFIELD_C2_CA_G2  | \
                                         SSL_LOAD_CERT_STARFIELD_S_RC_G2   | \
                                         SSL_LOAD_CERT_STARTCOM_CA         | \
                                         SSL_LOAD_CERT_STARTCOM_CA_G2      | \
                                         SSL_LOAD_CERT_SECOM_RCA2)

#define SSL_LOAD_CERT_OBSOLETE     (SSL_LOAD_CERT_RSA_SECURE_SERVER | \
                                         SSL_LOAD_CERT_VERISIGN_TSA_CA   | \
                                         SSL_LOAD_CERT_GTE_CT_ROOT)
#define SSL_LOAD_CERT_NOT_FOR_SERVER    (SSL_LOAD_CERT_CLASS1_PCA_G2_V2  | \
                                         SSL_LOAD_CERT_CLASS1_PCA_G3_V2  | \
                                         SSL_LOAD_CERT_CLASS1_PCA_SS_V4  | \
                                         SSL_LOAD_CERT_CLASS2_PCA_G2_V2  | \
                                         SSL_LOAD_CERT_CLASS2_PCA_G3_V2  | \
                                         SSL_LOAD_CERT_CLASS2_PCA_SS_V4  | \
                                         SSL_LOAD_CERT_CLASS4_PCA_G2_V2  | \
                                         SSL_LOAD_CERT_CLASS4_PCA_G3_V2)

#define SSL_LOAD_CERT_SCE01               (0x0000000000000001ULL)
#define SSL_LOAD_CERT_SCE02               (0x0000000000000002ULL)
#define SSL_LOAD_CERT_SCE03               (0x0000000000000004ULL)
#define SSL_LOAD_CERT_SCE04               (0x0000000000000008ULL)
#define SSL_LOAD_CERT_SCE05               (0x0000000000000010ULL)

#define SSL_LOAD_CERT_BALTIMORE_CT        (0x0000000000000020ULL)

#define SSL_LOAD_CERT_CLASS1_PCA_G2_V2    (0x0000000000000040ULL)
#define SSL_LOAD_CERT_CLASS1_PCA_G3_V2    (0x0000000000000080ULL)
#define SSL_LOAD_CERT_CLASS1_PCA_SS_V4    (0x0000000000000100ULL)

#define SSL_LOAD_CERT_CLASS2_PCA_G2_V2    (0x0000000000000200ULL)
#define SSL_LOAD_CERT_CLASS2_PCA_G3_V2    (0x0000000000000400ULL)
#define SSL_LOAD_CERT_CLASS2_PCA_SS_V4    (0x0000000000000800ULL)

#define SSL_LOAD_CERT_CLASS3_PCA_G2_V2    (0x0000000000001000ULL)
#define SSL_LOAD_CERT_CLASS3_PCA_G3_V2    (0x0000000000002000ULL)
#define SSL_LOAD_CERT_CLASS3_PCA_SS_V4    (0x0000000000004000ULL)

#define SSL_LOAD_CERT_CLASS4_PCA_G2_V2    (0x0000000000008000ULL)
#define SSL_LOAD_CERT_CLASS4_PCA_G3_V2    (0x0000000000010000ULL)

#define SSL_LOAD_CERT_ENTRUST_NET_SS_CA   (0x0000000000020000ULL)
#define SSL_LOAD_CERT_EQUIFAX_SEC_CA      (0x0000000000040000ULL)
#define SSL_LOAD_CERT_EQUIFAX_SEC_EBUS1   (0x0000000000080000ULL)

#define SSL_LOAD_CERT_GEOTRUST_GCA        (0x0000000000100000ULL)
#define SSL_LOAD_CERT_GLOBALSIGN_RCA      (0x0000000000200000ULL)

#define SSL_LOAD_CERT_GTE_CT_G_RCA        (0x0000000000400000ULL)
#define SSL_LOAD_CERT_GTE_CT_ROOT         (0x0000000000800000ULL)

#define SSL_LOAD_CERT_RSA1024_V1          (0x0000000001000000ULL)
#define SSL_LOAD_CERT_RSA2048_V3          (0x0000000002000000ULL)
#define SSL_LOAD_CERT_RSA_SECURE_SERVER   (0x0000000004000000ULL)
#define SSL_LOAD_CERT_THAWTE_PREM_SCA     (0x0000000008000000ULL)
#define SSL_LOAD_CERT_THAWTE_SCA          (0x0000000010000000ULL)
#define SSL_LOAD_CERT_VALICERT_CLASS2     (0x0000000020000000ULL)
#define SSL_LOAD_CERT_VERISIGN_TSA_CA     (0x0000000040000000ULL)
#define SSL_LOAD_CERT_AAA_CERT_SERVICES   (0x0000000080000000ULL)
#define SSL_LOAD_CERT_ADDTRUST_EXT_CA     (0x0000000100000000ULL)
#define SSL_LOAD_CERT_UTN_USERFIRST_HW    (0x0000000200000000ULL)
#define SSL_LOAD_CERT_EQUIFAX_SEC_G_EBUS1 (0x0000000400000000ULL)
#define SSL_LOAD_CERT_CLASS3_PCA_G5       (0x0000000800000000ULL)
#define SSL_LOAD_CERT_DIGICERT_HA_EV_RCA  (0x0000001000000000ULL)
#define SSL_LOAD_CERT_DIGICERT_A_ID_RCA   (0x0000002000000000ULL)
#define SSL_LOAD_CERT_DIGICERT_GLOBAL_RCA (0x0000004000000000ULL)
#define SSL_LOAD_CERT_CT_G_ROOT           (0x0000008000000000ULL)
#define SSL_LOAD_CERT_ENTRUST_CA2048      (0x0000010000000000ULL)
#define SSL_LOAD_CERT_ENTRUST_RCA         (0x0000020000000000ULL)
#define SSL_LOAD_CERT_GLOBALSIGN_RCA_R2   (0x0000040000000000ULL)
#define SSL_LOAD_CERT_GODADDY_CLASS2_CA   (0x0000080000000000ULL)
#define SSL_LOAD_CERT_STARFIELD_CLASS2_CA (0x0000100000000000ULL)
#define SSL_LOAD_CERT_STARFIELD_S_RC      (0x0000200000000000ULL)
#define SSL_LOAD_CERT_THATE_PRIMARY_RCA   (0x0000400000000000ULL)
#define SSL_LOAD_CERT_VALICERT_CLASS1_VA  (0x0000800000000000ULL)
#define SSL_LOAD_CERT_SECOM_RCA1          (0x0001000000000000ULL)
#define SSL_LOAD_CERT_SECURE_TRUST_CA     (0x0002000000000000ULL)
#define SSL_LOAD_CERT_ENTRUST_RCA_G2      (0x0004000000000000ULL)
#define SSL_LOAD_CERT_VERISIGN_U_RCA      (0x0008000000000000ULL)
#define SSL_LOAD_CERT_GODADDY_C2_CA_G2    (0x0010000000000000ULL)
#define SSL_LOAD_CERT_STARFIELD_C2_CA_G2  (0x0020000000000000ULL)
#define SSL_LOAD_CERT_STARFIELD_S_RC_G2   (0x0040000000000000ULL)
#define SSL_LOAD_CERT_STARTCOM_CA         (0x0080000000000000ULL)
#define SSL_LOAD_CERT_STARTCOM_CA_G2      (0x0100000000000000ULL)
#define SSL_LOAD_CERT_SECOM_RCA2          (0x0200000000000000ULL)


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef void* sslCert;
typedef void* sslCertName;


int sslInit(void *pool, uint32_t poolSize);
int sslEnd(void);

/* SSL certificate loader */
int sslCertificateLoader(uint64_t flag, char *buffer, uint32_t size, uint32_t *required);

/* SSL certificate information get functions */
int sslCertGetSerialNumber(const sslCert cert, const uint8_t **sboData, uint32_t *sboLength);
int sslCertGetPublicKey(const sslCert cert, const uint8_t **sboData, uint32_t *sboLength);
int sslCertGetRsaPublicKeyModulus(const sslCert cert, const uint8_t **sboData, uint32_t *sboLength);
int sslCertGetRsaPublicKeyExponent(const sslCert cert, const uint8_t **sboData, uint32_t *sboLength);

//int sslCertGetNotBefore(const sslCert cert, CellRtcTick *begin);
//int sslCertGetNotAfter(const sslCert cert, CellRtcTick *limit);
int sslCertGetSubjectName(const sslCert cert, const sslCertName *name);
int sslCertGetIssuerName(const sslCert cert, const sslCertName *name);

int sslCertGetNameEntryCount(const sslCert cert, uint32_t *entryCount);
int sslCertGetNameEntryInfo(const sslCert cert, uint32_t entryNum, const char **oidName, const uint8_t **value, uint32_t *valueLength, int32_t flag);
int sslCertGetMd5Fingerprint(const sslCert cert, const uint8_t *buf, uint32_t *plen);


#ifdef __cplusplus
	}
#endif /* __cplusplus */

#endif /* __SSL_H__ */
