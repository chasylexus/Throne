#pragma once

#include <QIcon>
#include <QMenu>
#include <QObject>
#include <QSystemTrayIcon>

// QSystemTrayIcon everywhere except macOS, where the status item is native: Qt 6.11.2
// asks the current event for clickCount when the status item menu starts tracking, and
// on macOS 27 that event is a gesture, so AppKit asserts and the app aborts. Fixed in
// the Qt 6.11 branch (2026-08-04) but not in a release yet.
class TrayIcon : public QObject {
    Q_OBJECT
public:
    explicit TrayIcon(QObject *parent = nullptr);
    ~TrayIcon() override;

    void setIcon(const QIcon &icon);
    void setToolTip(const QString &text);
    void setContextMenu(QMenu *menu);
    void setVisible(bool visible);
    void hide() { setVisible(false); }
    bool isVisible() const;

signals:
    void activated(QSystemTrayIcon::ActivationReason reason);

private:
#ifdef Q_OS_MAC
    void *m_statusItem = nullptr; // NSStatusItem, kept opaque so this header stays C++
    bool m_visible = false;
#else
    QSystemTrayIcon *m_tray;
#endif
};
