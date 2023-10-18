
#ifndef MainApp_H
#define MainApp_H

class MainApp : public wxApp {
    public:
        virtual bool OnInit();
};

DECLARE_APP(MainApp)

#endif