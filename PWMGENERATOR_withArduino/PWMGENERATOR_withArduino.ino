
void setup() {
  // put your setup code here, to run once:

pinMode(5,OUTPUT);

pinMode(6,OUTPUT);

TCCR0A = 0; //RESET REGISTER // general 4mular for pwm frequency computation F = f_cll /(N*256); N is prescaler
TCCR0B = 0; //RESET REGISTER //
TCCR0A = 0b01010010;//FAST PWM MODE
TCCR0B = 0b00000001; //PRESCALER 64 formula for frequency base on 0CR0A Focr0a = Fclk/2*N*(1+ocr0a)
OCR0A = 180;//control value //DUTY CYCLE FOR PIN 6  //8MHz 50% duty cycle 
OCR0B = 180; //DUTY CYCLE FOR PIN 5 //note 0cr0a cant be more than 255 bcos it is 8bits
//if Ocroa = 50 we av 156Khz,
}
void loop() {
  // put your main code here, to run repeatedly:

}
