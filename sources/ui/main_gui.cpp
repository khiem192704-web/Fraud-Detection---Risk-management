#include <QApplication>
#include "ui/LoginDialog.h"
#include "ui/MainWindow.h"
#include "bll/Auth.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("EPFD-RAS GUI");

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
