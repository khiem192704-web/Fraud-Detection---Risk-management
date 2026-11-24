#include <QApplication>
#include <QFont>
#include "ui/LoginDialog.h"
#include "ui/MainWindow.h"
#include "bll/Auth.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("PBL2");

    // Thiết lập Font chữ tiêu chuẩn hiện đại Segoe UI với khử răng cưa mượt mà
    QFont appFont("Segoe UI", 10);
    appFont.setStyleHint(QFont::SansSerif);
    appFont.setStyleStrategy(QFont::PreferAntialias);
    app.setFont(appFont);

    AuthService authService;
    authService.loadFromDisk("data/admin.txt");

    while (true) {
        LoginDialog loginDlg(&authService);
        if (loginDlg.exec() != QDialog::Accepted) {
            break; // Thoát nếu người dùng đóng hộp thoại đăng nhập
        }

        MainWindow window(&authService);
        window.show();
        int ret = app.exec();

        // Nếu MainWindow đóng mà không phải do Admin bấm Đăng xuất -> Thoát hẳn
        if (!window.isLoggedOut()) {
            return ret;
        }
        // Nếu vừa đăng xuất -> quay lại vòng lặp hiển thị LoginDialog!
    }

    return 0;
}
