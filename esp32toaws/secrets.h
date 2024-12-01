#include <pgmspace.h>
const char* ssid ="Linus iPhone";
const char* password="Lajnus2003!";
const char* AWS_IOT_ENDPOINT="xxxxxxxx.amazons.com";

//Amazon Root CA 1 
static const char AWS_CERT_CA[] PROGMEM =R"EOF(
  -----BEGIN CERTIFICATE-----

  -----END CERTIFICATE-----
)EOF";


//Device Certificate
static const char AWS_CERT_CRT[] PROGMEM =R"KEY(
  -----BEGIN CERTIFICATE-----

  -----END CERTIFICATE-----
)KEY";


//Deivce Private Key 
static const char AWS_CERT_PRIVATE[] PROGMEM =R"KEY(
  -----BEGIN RSA PRIVATE KEY-----

  -----END RSA PRIVATE KEY -----
)KEY";