#ifndef _INPUTCONTROLLER_H_
#define _INPUTCONTROLLER_H_

/**
 * @brief InputController
 * @ingroup Controller
 **/
template <typename T>
class InputController {
  public:
    /**
     * @brief Callback definition for onSelectInput function
     * 
     * Gets called when device receive a `selectInput` request \n
     * @param[in]   deviceId    String which contains the ID of device
     * @param[in]   input       String with input name device is requested to switch to \n `AUX 1`..`AUX 7`, `BLURAY`, `CABLE`, `CD`, `COAX 1`,`COAX 2`, `COMPOSITE 1`, `DVD`, `GAME`, `HD RADIO`, `HDMI 1`.. `HDMI 10`, `HDMI ARC`, `INPUT 1`..`INPUT 10`, `IPOD`, `LINE 1`..`LINE 7`, `MEDIA PLAYER`, `OPTICAL 1`, `OPTICAL 2`, `PHONO`, `PLAYSTATION`, `PLAYSTATION 3`, `PLAYSTATION 4`, `SATELLITE`, `SMARTCAST`, `TUNER`, `TV`, `USB DAC`, `VIDEO 1`..`VIDEO 3`, `XBOX`
     * @param[out]  input       String with input name device has switchted to \n `AUX 1`..`AUX 7`, `BLURAY`, `CABLE`, `CD`, `COAX 1`,`COAX 2`, `COMPOSITE 1`, `DVD`, `GAME`, `HD RADIO`, `HDMI 1`.. `HDMI 10`, `HDMI ARC`, `INPUT 1`..`INPUT 10`, `IPOD`, `LINE 1`..`LINE 7`, `MEDIA PLAYER`, `OPTICAL 1`, `OPTICAL 2`, `PHONO`, `PLAYSTATION`, `PLAYSTATION 3`, `PLAYSTATION 4`, `SATELLITE`, `SMARTCAST`, `TUNER`, `TV`, `USB DAC`, `VIDEO 1`..`VIDEO 3`, `XBOX`
     * @return      the success of the request
     * @retval      true        request handled properly
     * @retval      false       request was not handled properly because of some error
     * 
     * @section SelectInput Example-Code
     * @snippet callbacks.cpp onSelectInput
     **/
    using SelectInputCallback = std::function<bool(const String &, String &)>;

    void onSelectInput(SelectInputCallback cb);
    bool sendSelectInputEvent(String intput, String cause = "PHYSICAL_INTERACTION");

  protected:
    bool handleInputController(const String &action, JsonObject &request_value, JsonObject &response_value);

  private: 
    SelectInputCallback selectInputCallback;
};

/**
 * @brief Set callback function for `selectInput` request
 * 
 * @param cb Function pointer to a `SelectInputCallback` function
 * @return void
 * @see SelectInputCallback
 **/
template <typename T>
void InputController<T>::onSelectInput(SelectInputCallback cb) {
  selectInputCallback = cb;
}

/**
 * @brief Send `selectInput` event to SinricPro Server to report selected input
 * 
 * @param input           String with actual media control \n `AUX 1`..`AUX 7`, `BLURAY`, `CABLE`, `CD`, `COAX 1`,`COAX 2`, `COMPOSITE 1`, `DVD`, `GAME`, `HD RADIO`, `HDMI 1`.. `HDMI 10`, `HDMI ARC`, `INPUT 1`..`INPUT 10`, `IPOD`, `LINE 1`..`LINE 7`, `MEDIA PLAYER`, `OPTICAL 1`, `OPTICAL 2`, `PHONO`, `PLAYSTATION`, `PLAYSTATION 3`, `PLAYSTATION 4`, `SATELLITE`, `SMARTCAST`, `TUNER`, `TV`, `USB DAC`, `VIDEO 1`..`VIDEO 3`, `XBOX`
 * @param cause           (optional) `String` reason why event is sent (default = `"PHYSICAL_INTERACTION"`)
 * @return the success of sending the even
 * @retval true   event has been sent successfully
 * @retval false  event has not been sent, maybe you sent to much events in a short distance of time
 **/
template <typename T>
bool InputController<T>::sendSelectInputEvent(String input, String cause) {
  T& device = static_cast<T&>(*this);

  DynamicJsonDocument eventMessage = device.prepareEvent("selectInput", cause.c_str());
  JsonObject event_value = eventMessage["payload"]["value"];
  event_value["input"] = input;
  return device.sendEvent(eventMessage);
}

template <typename T>
bool InputController<T>::handleInputController(const String &action, JsonObject &request_value, JsonObject &response_value) {
  T &device = static_cast<T &>(*this);

  bool success = false;

  if (selectInputCallback && action == "selectInput") {
    String input = request_value["input"];
    success = selectInputCallback(device.deviceId, input);
    response_value["input"] = input;
    return success;
  }

  return success;
}

#endif