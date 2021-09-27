#include <hidcomposite.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>


// Override HIDComposite to be able to select which interface we want to hook into
class HIDSelector : public HIDComposite
{
public:
    HIDSelector(USB *p) : HIDComposite(p) {};

protected:
    void ParseHIDData(USBHID *hid, uint8_t ep, bool is_rpt_id, uint8_t len, uint8_t *buf);
    bool SelectInterface(uint8_t iface, uint8_t proto);
};


bool HIDSelector::SelectInterface(uint8_t iface, uint8_t proto)
{
    return true;
}


// Will be called for all HID data received from the USB interface
void HIDSelector::ParseHIDData(USBHID *hid, uint8_t ep, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
    if (len && buf)  {
        Serial.print(F("\r\nEp"));
        Serial.print(ep);
        Serial.print(F(": "));
        for (uint8_t i = 0; i < len; i++) {
            if (buf[i] < 16) Serial.print(F("0"));
            Serial.print(buf[i], HEX);
            Serial.print(F(" "));
        }
    }
}


USB         Usb;
USBHub      Hub(&Usb);
HIDSelector hidSelector(&Usb);

void setup()
{
    Serial.begin( 115200 );
#if !defined(__MIPSEL__)
    while (!Serial);
#endif
    Serial.println("Start");

    if (Usb.Init() == -1)
        Serial.println("OSC did not start.");

    delay( 200 );
}

static uint8_t prev_state = 0;

void loop()
{
    Usb.Task();

    // USB state
    if (prev_state != Usb.getUsbTaskState()) {
        prev_state = Usb.getUsbTaskState();
        Serial.print(F("usb_state: "));
        Serial.println(Usb.getUsbTaskState(), HEX);
    }
}
