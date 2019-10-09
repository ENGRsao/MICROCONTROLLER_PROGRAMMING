enum {ssid, password,ip,hotspot,hotspot_pass};

 const char esp_ssid[]PROGMEM = "PowerStore1";
 const char esp_password[]PROGMEM=  "12345678";
 const char esp_ip[]PROGMEM = "192.168.1.1";
 const char esp_hotspot[]PROGMEM ="WIFI"
 const char esp_hotspot_pass[]PROGMEM ="thinkact&lead";
 const char* const esp_data[] PROGMEM = {esp_ssid,esp_password,esp_ip,esp_hotspot,esp_hotspot_pass};

  enum {e_check,e_reset,e_server,e_mode,e_disconnect,e_type,e_ip,e_config,e_send,e_status,e_start,e_join}
  const char espcheck[]PROGMEM = "AT";
  const char espreset[]PROGMEM = "AT+RST";
  const char esp_create_server[]PROGMEM = "AT+CIPSERVER=1,80";
  const char esp_mode[] PROGMEM = "AT+CWMODE";
  const char esp_disconnect[] PROGMEM = "AT+CWQAP";
  const char esp_connectiontype[] PROGMEM = "AT+CIPMUX=1";
  const char esp_ip_address[] PROGMEM = "AT+CIPAP";
  const char esp_config[] PROGMEM = "AT+CWSAP";
  const char esp_send[] PROGMEM = "AT+CIPSEND";
  const char esp_status[] PROGMEM = "AT+CIPSTATUS";
  const char esp_start[] PROGMEM = "AT+CIPSTART=\"TCP\",";
  const char esp_join[] PROGMEM = "AT+CWJAP";
  const char* const esp_commands[] PROGMEM = {espcheck,espreset,esp_create_server,esp_mode,esp_disconnect,esp_connectiontype,esp_ip_address,esp_config,esp_send,esp_status,esp_start,esp_join};
  
  enum {s_check,s_reset,s_apn,s_gprs,s_ssl,s_shut,s_start,s_config,s_status,s_send}
  const char simcheck[]PROGMEM = "AT";
  const char simreset[]PROGMEM = "AT+RST";
  const char simapn[]PROGMEM = "AT+CSTT";
  const char simgprs[] PROGMEM = "AT+CIICR";
  const char simssl[] PROGMEM = "AT+CIPSSL";
  const char simshut[] PROGMEM = "AT+CIPSHUT";
  const char simstart[] PROGMEM = "AT+CIPSTART=\"TCP\",";
  const char simconfig[] PROGMEM = "AT+CIFSR";
  const char simstatus_check[] PROGMEM = "AT+CIPSTATUS";
  const char sim_send[] PROGMEM = "AT+CIPSEND";
  const char* const sim_commands[] PROGMEM = {simcheck,simreset,simapn,simgprs,simssl,simshut,simstart,sim_config,simstatus_check,sim_send};

char* get_Ram_Data(const char* Table_Details[],int i)
{
   strcpy_P(buffer,(char*) pgm_read_word(&Table_Details[i]));
   return buffer;
}
