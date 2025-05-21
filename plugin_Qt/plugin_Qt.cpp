#include <cassert>
#include <iostream>
#include <cstdint>            // uintptr_t
#include <QWindow>
#include <QMessageBox>
#include <QString>
#include <QPushButton>
#include <QGridLayout>
#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QSlider>
#include <QCheckBox>
#include <QFormLayout>

#include <mutex>
#include <optional>
#include <thread>

#if 0

int g_x = 0, g_y = 0, g_w = 0, g_h = 0;

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <QColor>

#include "../wxModularCore/PixelContainer.hpp"

QWidget *parentWidget = nullptr;

// Function to render widgets into a QPixmap and extract the pixel data
extern "C" __declspec(dllexport) void RenderWidgets(PixelContainer &pixelData, unsigned const w, unsigned const h)
{
    assert( nullptr != &pixelData );  // just for debug

    parentWidget->setGeometry(0, 0, w, h);

    QPixmap pixmap( parentWidget->size() );
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    parentWidget->render(&painter);
    painter.end();

    QImage image = pixmap.toImage();
    assert( w == image.width () );
    assert( h == image.height() );
    pixelData.ClearAndResize(w, h);

    for ( int x = 0; x < w; ++x )
    {
        for ( int y = 0; y < h; ++y )
        {
            pixelData[x][y] = image.pixel(x, y);
        }
    }
}

extern "C" __declspec(dllexport) bool CreatePluginPixels(void)
{
    //std::cout << "Entered function: DrawWidgets\n";

    int argc = 1;
    char *argv_buf[2] = { (char*)"program", nullptr };
    char **argv = argv_buf;

    static QApplication *app = nullptr;
    static QPushButton *button = nullptr;

    static std::once_flag myflag;
    bool active = false;
    std::call_once(myflag,
       [&]()
       {
           active = true;
           app = new QApplication(argc,argv);
           parentWidget = new QWidget;
           parentWidget->setWindowFlags(Qt::FramelessWindowHint);
           parentWidget->setAttribute(Qt::WA_TranslucentBackground);

           QLabel *const usernameLabel = new QLabel("Username:");
           QLineEdit *const usernameLineEdit = new QLineEdit();
           usernameLineEdit->setPlaceholderText("Enter username");

           QLabel *const passwordLabel = new QLabel("Password:");
           QLineEdit *const passwordLineEdit = new QLineEdit();
           passwordLineEdit->setPlaceholderText("Enter password");
           passwordLineEdit->setEchoMode(QLineEdit::Password);

           QLabel *const amplificationLabel = new QLabel("Amplification:");
           QSlider *const amplificationSlider = new QSlider(Qt::Horizontal);
           amplificationSlider->setRange(0, 100);
           amplificationSlider->setTickPosition(QSlider::TicksBelow);
           amplificationSlider->setTickInterval(10);
           amplificationSlider->setValue(65);

           QLabel *const reverbLabel = new QLabel("Reverb:");
           QCheckBox *const reverbCheckBox = new QCheckBox("Enable Reverb");

           QPushButton *const submitButton = new QPushButton("Submit");

           QFormLayout *const formLayout = new QFormLayout;
           formLayout->addRow(usernameLabel, usernameLineEdit);
           formLayout->addRow(passwordLabel, passwordLineEdit);

           // Create a layout for amplification and reverb controls
           QVBoxLayout *const controlLayout = new QVBoxLayout;
           controlLayout->addWidget(amplificationLabel);
           controlLayout->addWidget(amplificationSlider);
           controlLayout->addWidget(reverbLabel);
           controlLayout->addWidget(reverbCheckBox);

           // Create a vertical layout and add the form and control layouts
           QVBoxLayout *const mainLayout = new QVBoxLayout;
           mainLayout->addLayout(formLayout);    // Add the form layout (username/password)
           mainLayout->addLayout(controlLayout); // Add the control layout (slider and checkbox)
           mainLayout->addWidget(submitButton);  // Add the submit button

           // Set the layout for the main window
           parentWidget->setLayout(mainLayout);
           parentWidget->connect( submitButton, &QPushButton::clicked, parentWidget,
             []()
             {
                 QMessageBox::information(nullptr,"Monkey", "Frog");
             } );
       });

    // The following line would set up the event loop
    // but magically we don't need it -- I don't
    // know why!
    // if ( active ) app->exec();

    return true;
}

#elif 0

extern "C" bool PopulateScreenCoord(int const x, int const y, int const w, int const h)
{
    if ( g_x==x && g_y==y && g_w==w && g_h==h ) return true;

    //std::cout << "Entered function: DrawWidgets\n";

    int argc = 1;
    char *argv_buf[2] = { (char*)"program", nullptr };
    char **argv = argv_buf;

    static QApplication *app = nullptr;
    static QPushButton *button = nullptr;

    static std::once_flag myflag;
    bool active = false;
    std::call_once(myflag,
        [&]()
        {
            active = true;
            app = new QApplication(argc,argv);
            parentWidget = new QWidget;
            parentWidget->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
            parentWidget->setAttribute(Qt::WA_TranslucentBackground);
            button = new QPushButton( "Click me!", parentWidget );
            parentWidget->connect( button, &QPushButton::clicked, parentWidget,
              []()
              {
                  QMessageBox::information(nullptr,"Monkey", "Frog");
              } );
            auto *const layout = new QVBoxLayout;
            layout->addWidget(button);
            parentWidget->setLayout(layout);
        });

    parentWidget->setGeometry(x, y, w, h);

    // The following line would set up the event loop
    // but magically we don't need it -- I don't
    // know why!
    // if ( active ) app->exec();

    return true;
}

#else

extern "C" __declspec(dllexport) bool PopulatePanelNativeHandle(void *const handle)
{
    assert( nullptr != handle );

    //std::cout << "Entered function: DrawWidgets\n";

    int argc = 1;
    char *argv_buf[2] = { (char*)"program", nullptr };
    char **argv = argv_buf;

    static QApplication *app = nullptr;
    static QWidget *parentWidget = nullptr;
    static QPushButton *button = nullptr;

    static std::once_flag myflag;
    bool active = false;
    std::call_once(
        myflag,
       [&]()
       {
           active = true;
           app = new QApplication(argc,argv);
           auto *const qtWin = QWindow::fromWinId( (std::uintptr_t)handle );
#if 0
           auto *const grandparentWidget = QWidget::createWindowContainer(qtWin);
           grandparentWidget->winId();
           grandparentWidget->windowHandle()->setParent(qtWin);
           parentWidget = new QWidget(grandparentWidget);
#else
           parentWidget = QWidget::createWindowContainer(qtWin);
#endif
           //parentWidget->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
           //parentWidget->setAttribute(Qt::WA_TranslucentBackground);
           button = new QPushButton( "Click me!", parentWidget );
           parentWidget->connect(button, &QPushButton::clicked, parentWidget,
                                 []()
                                 {
                                     QMessageBox::information(nullptr,"Monkey", "Frog");
                                 } );
           auto *const layout = new QVBoxLayout;
           layout->addWidget(button);
           parentWidget->setLayout(layout);
#if 0
           grandparentWidget->show();
#else
           parentWidget->show();
#endif
       });

    //parentWidget->setGeometry(x, y, w, h);

    // The following line would set up the event loop
    // but magically we don't need it -- I don't
    // know why!
    // if ( active ) app->exec();

    return true;
}

#endif
