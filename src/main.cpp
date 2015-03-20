#include <iostream>
#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WImage>
//#include <Wt/WBootstrapTheme>
//#include <Wt/WTheme>
//#include <Wt/WCssTheme>
#include <Wt/WServer>
#include <Wt/WResource>
#include <Wt/WStreamResource>
#include <Wt/WFileResource>
#include <Wt/Http/Request>
#include <Wt/Http/Response>
#include <Wt/Dbo/Dbo>
#include <Wt/Auth/AuthWidget>
#include <Wt/Auth/AbstractUserDatabase>
#include <Wt/Auth/PasswordService>
#include <Wt/Auth/AuthModel>
#include <Wt/WAnchor>
#include <Wt/WObject>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <libavutil/mathematics.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#include "Session.h"



// Debian packages needed: libwt-dev libwt-doc libopencv-dev libwtdbo-dev libwtext-dev libwthttp-dev libwtdbosqlite-dev
// Run with: --http-port 8080 --http-address localhost --docroot 'docroot' --approot '.'
// Stream video vlc v4l2:///dev/video0:size640x480
// vlc -I dummy -v --noaudio --ttl 12 v4l2:///dev/video0:size=640x480  --sout '#std{access=mmsh,dst=:8081}' -V X11

cv::VideoCapture *cap = nullptr;


class TakePicWidget : public Wt::WContainerWidget {
public:

    void takePic() {
       Wt::log("info") << "Take picture";
        cv::Mat picture;

        // Deal with 5 frame latency
        Wt::log("info") << "grab 5 frames";
        for (int i = 0; i < 5; i++) {
            cap->grab();
        }

        Wt::log("info") << "Retrieve last frame";
        *cap >> picture;

        if (! fileName.empty()) {
            unlink(fileName.c_str());
        }

        fileName = "/tmp/cap-" + std::to_string(time(nullptr)) + ".jpg";

        Wt::log("info") << "Write frame to " << fileName;
        cv::imwrite(fileName, picture);

        Wt::log("info") << "Set file as source for widget";
        lastImage->setResource(new Wt::WFileResource(fileName, this));
        lastImage->show();
    }

    //===============================================================

    TakePicWidget ( WContainerWidget* parent = 0 ) {
        Wt::log("info") << "Pic widget constructor";

        lastImage = new Wt::WImage(this);
        lastImage->hide();
        Wt::WPushButton *button = new Wt::WPushButton("Take pic", this);
        addWidget(new Wt::WBreak(this));
        button->clicked().connect(this, &TakePicWidget::takePic);


        if (! cap) {
           Wt::log("info") << "New capture object";
           cap = new cv::VideoCapture(0);
        }

        if (! cap->isOpened()) {
           Wt::log("warning") << "Failed to open capture object";
            Wt::log("warn") << "Cannot open video capture device";
            button->setText("Capture device not available");
            button->disable();
        } else {
           Wt::log("info") << "Set output size";
            cap->set(CV_CAP_PROP_FRAME_HEIGHT, 480);
            cap->set(CV_CAP_PROP_FRAME_WIDTH, 640);

            Wt::log("info") << "20 warmup frames";
            for (int i = 0; i < 20; i++) {
                cap->grab();
            }

            takePic();
        }
    }


    //===============================================================

    ~TakePicWidget() {
        if (cap) {
            cap->release();
            cap = nullptr;
        }
    }

private:
    Wt::WImage *lastImage;
    std::string fileName;

};


//===============================================================

class HelloApplication : public Wt::WApplication
{
public:
    HelloApplication(const Wt::WEnvironment& env);
    void authEvent();

    Wt::Auth::AuthWidget *authWidget;

private:
    TakePicWidget *takePicWidget;
    Session session;
};



//===============================================================

HelloApplication::HelloApplication(const Wt::WEnvironment& env)
        : Wt::WApplication(env), session(appRoot() + "homesec.db")
{
    setTitle("Hello world");
    Wt::log("info") << "App Root: " << appRoot();
    root()->addStyleClass("container");
    //setTheme(new Wt::WBootstrapTheme());
    useStyleSheet("css/style.css");


    // If no user 'pi' registered, register a new one
    Wt::Auth::User user =  session.getUsers().findWithIdentity(Wt::Auth::Identity::LoginName, "pi");

    if (! user.isValid()) {
       session.registerUser("pi", "123456");
       }


    authWidget
            = new Wt::Auth::AuthWidget(Session::getBaseAuth(), session.getUsers(),
                               session.getLogin());

    takePicWidget = new TakePicWidget(root());
    takePicWidget->hide();

    session.getLogin().changed().connect(this, &HelloApplication::authEvent);

    authWidget->model()->addPasswordAuth(&Session::getPasswordAuth());
    authWidget->model()->addOAuth(Session::getOAuthServices());
    authWidget->setRegistrationEnabled(false);
    authWidget->processEnvironment();
    root()->addWidget(authWidget);
    root()->addWidget(takePicWidget);


}

//===============================================================

void HelloApplication::authEvent() {
   Wt::log("info") << "Auth event";

   if (session.getLogin().loggedIn()) {
        const Wt::Auth::User& u = session.getLogin().user();
        Wt::log("notice")
        << "User " << u.id()
        << " (" << u.identity(Wt::Auth::Identity::LoginName) << ")"
        << " logged in.";

        takePicWidget->show();

    } else {
        Wt::log("notice") << "User logged out.";
        takePicWidget->hide();
    }
}



//===============================================================

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
    Wt::WApplication * app = new HelloApplication(env);

    return app;
}

//===============================================================

class TrialResfulResource : public Wt::WResource {

public:
    TrialResfulResource(Wt::WObject *parent = 0) : Wt::WResource(parent) {

    }

    ~TrialResfulResource() {
        beingDeleted();
    }

    void handleRequest(const Wt::Http::Request& request,
                       Wt::Http::Response& response) {
        response.setMimeType("text/html");
        response.out() << "<html><body>Resource called: " << request.method() << "</body></html>";
    }


};

//===============================================================

int main(int argc, char **argv)
{
    Wt::WServer server(argv[0]);
    server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
    server.addEntryPoint(Wt::Application, createApplication);
    server.addResource(new TrialResfulResource(), "/api");
    Session::configureAuth();



    server.start();
    return server.waitForShutdown();
}
