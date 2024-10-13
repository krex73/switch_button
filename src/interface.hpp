#include <GyverPortal.h>
#include <ArduinoJson.h>
extern GyverPortal ui;

void build()
{
  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);

  // ui.uri() возвращает текущий адрес страницы
  // используем его в конструкции с if для постройки страницы
  // переход на страницы может осуществляться в адресной строке браузера
  // или по кнопке-ссылке BUTTON_LINK

  // страница с формой
  // ВАЖНО: url страницы должен совпадать с именем формы!
  // чтобы форма отображалась в браузере после Submit
  GP.NAV_TABS_LINKS("/,/log,/settings", "ТекущиЕ показания,Утилиты,Настройки");

  // страница с кнопкой, на которую можно кликнуть
  if (ui.uri("/settings"))
  {

    GP.FORM_BEGIN("/settings");
    GP.LABEL("Сеть Wi-Fi к которой додключаемся");
    GP.BREAK();
    GP.LABEL("Имя точки");
    GP.TEXT("ssid", "Имя сети", config.ssid);
    GP.BREAK();
    GP.LABEL("Пароль");
    GP.TEXT("pass", "Пароль", config.pass);
    GP.BREAK();
    GP.LABEL("Создавемая точка доступа Wi-Fi,");
    GP.BREAK();
    GP.LABEL("если не подключимся");
    GP.BREAK();
    GP.LABEL("Имя точки");
    GP.TEXT("ssid_td", "Имя сети", config.ssid_td);
    GP.BREAK();
    GP.LABEL("Пароль");
    GP.TEXT("pass_td", "Пароль", config.pass_td);
    GP.BREAK();

    GP.SUBMIT("Сохранить");
    GP.FORM_END();
  }
  else if (ui.uri("/log"))
  {
    GP.LABEL("Утилиты");
    GP.BUTTON_LINK("/log", "Поиск датчиков");
    GP.LABEL("Перезагрузка");
    GP.BUTTON_LINK("/reset", "Перезагрузить");
  }
  else
  {
    GP.LABEL("Home");
  }

  GP.BUILD_END();
}

bool led;
void action()
{

  if (ui.form())
  {
    if (ui.formName().equals("/settings"))
    {

      config.ssid = ui.getString("ssid");
      config.pass = ui.getString("pass");
      config.ssid_td = ui.getString("ssid_td");
      config.pass_td = ui.getString("pass_td");
      saveConfig();
      ESP.reset();
    }
  }

  if (ui.uri("/settings"))
  {
  }
  else if (ui.uri("/log"))
  {
    // findDevicesAndLog();
  }
  else if (ui.uri("/reset"))
  {
    ESP.reset();
  }
}
