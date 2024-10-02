#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QTimer>
#include <QTime>
#include <QDate>

class ZamanlayiciUygulamasi : public QWidget {
public:
    ZamanlayiciUygulamasi() {
        arayuzHazirla();
        kronometreCalisiyor = false;
        gecenZaman = 0;
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &ZamanlayiciUygulamasi::kronometreGuncelle);

        geriSayimTimer = new QTimer(this);
        connect(geriSayimTimer, &QTimer::timeout, this, &ZamanlayiciUygulamasi::geriSayimGuncelle);

        saatTimer = new QTimer(this);
        connect(saatTimer, &QTimer::timeout, this, &ZamanlayiciUygulamasi::saatGuncelle);
        saatTimer->start(1000);

        tarihTimer = new QTimer(this);
        connect(tarihTimer, &QTimer::timeout, this, &ZamanlayiciUygulamasi::tarihGuncelle);
        tarihTimer->start(5000);
    }

private:
    QLabel *kronometreEtiketi;
    QLabel *geriSayimEtiketi;
    QLabel *saatEtiketi;
    QLabel *tarihEtiketi;
    QLineEdit *zamanGiris;
    QTimer *timer;
    QTimer *geriSayimTimer;
    QTimer *saatTimer;
    QTimer *tarihTimer;
    bool kronometreCalisiyor;
    int gecenZaman;
    int kalanZaman;

    void arayuzHazirla() {
        setWindowTitle("Zamanlayıcı");
        QVBoxLayout *duzen = new QVBoxLayout;

        kronometreEtiketi = new QLabel("Kronometre: 00:00");
        duzen->addWidget(kronometreEtiketi);

        QPushButton *baslaButonu = new QPushButton("Başla");
        connect(baslaButonu, &QPushButton::clicked, this, &ZamanlayiciUygulamasi::kronometreBasla);
        duzen->addWidget(baslaButonu);

        QPushButton *durdurButonu = new QPushButton("Durdur");
        connect(durdurButonu, &QPushButton::clicked, this, &ZamanlayiciUygulamasi::kronometreDurdur);
        duzen->addWidget(durdurButonu);

        QPushButton *resetButonu = new QPushButton("Reset");
        connect(resetButonu, &QPushButton::clicked, this, &ZamanlayiciUygulamasi::kronometreSifirla);
        duzen->addWidget(resetButonu);

        geriSayimEtiketi = new QLabel("Zamanlayıcı: 00:00");
        duzen->addWidget(geriSayimEtiketi);

        zamanGiris = new QLineEdit(this);
        zamanGiris->setPlaceholderText("Dakika.Saniye girin (örneğin 1.20)");
        duzen->addWidget(zamanGiris);

        QPushButton *geriSayimBaslaButonu = new QPushButton("Zamanlayıcıyı Başlat");
        connect(geriSayimBaslaButonu, &QPushButton::clicked, this, &ZamanlayiciUygulamasi::geriSayimBasla);
        duzen->addWidget(geriSayimBaslaButonu);

        saatEtiketi = new QLabel("");
        saatEtiketi->setStyleSheet("font-size: 24px;");
        duzen->addWidget(saatEtiketi);

        tarihEtiketi = new QLabel("");
        tarihEtiketi->setStyleSheet("font-size: 16px;");
        duzen->addWidget(tarihEtiketi);

        setLayout(duzen);
        saatGuncelle();
        tarihGuncelle();
    }

    void kronometreBasla() {
        if (!kronometreCalisiyor) {
            kronometreCalisiyor = true;
            timer->start(1000);
        }
    }

    void kronometreDurdur() {
        if (kronometreCalisiyor) {
            kronometreCalisiyor = false;
            timer->stop();
        }
    }

    void kronometreSifirla() {
        kronometreCalisiyor = false;
        timer->stop();
        gecenZaman = 0;
        kronometreEtiketi->setText("Kronometre: 00:00");
    }

    void kronometreGuncelle() {
        gecenZaman++;
        kronometreEtiketi->setText("Kronometre: " + formatZaman(gecenZaman));
    }

    void geriSayimBasla() {
        QString zamanGirisText = zamanGiris->text();
        QStringList zamanParcalari = zamanGirisText.split('.');
        if (zamanParcalari.size() == 2) {
            bool dakikalarOk, saniyelerOk;
            int dakikalar = zamanParcalari[0].toInt(&dakikalarOk);
            int saniyeler = zamanParcalari[1].toInt(&saniyelerOk);
            if (dakikalarOk && saniyelerOk && dakikalar >= 0 && saniyeler >= 0 && saniyeler < 60) {
                kalanZaman = dakikalar * 60 + saniyeler;
                geriSayimEtiketi->setText("Zamanlayıcı: " + formatZaman(kalanZaman));
                geriSayimTimer->start(1000);
            } else {
                QMessageBox::warning(this, "Hata", "Lütfen geçerli bir dakika.saniye formatında değer girin (örn. 1.20).");
            }
        }
    }

    void geriSayimGuncelle() {
        if (kalanZaman > 0) {
            kalanZaman--;
            geriSayimEtiketi->setText("Zamanlayıcı: " + formatZaman(kalanZaman));
        } else {
            geriSayimTimer->stop();
            // Ses çalmayı buraya ekleyebilirsiniz
            QMessageBox::information(this, "Bitti", "Zamanlayıcı süresi doldu!");
        }
    }

    void saatGuncelle() {
        QString mevcutSaat = QTime::currentTime().toString("hh:mm:ss");
        saatEtiketi->setText("Güncel Saat: " + mevcutSaat);
    }

    void tarihGuncelle() {
        QString mevcutTarih = QDate::currentDate().toString("dddd, d MMMM yyyy");
        tarihEtiketi->setText("Güncel Tarih: " + mevcutTarih);
    }

    QString formatZaman(int saniye) {
        int dakika = saniye / 60;
        saniye = saniye % 60;
        return QString::number(dakika).rightJustified(2, '0') + ":" + QString::number(saniye).rightJustified(2, '0');
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ZamanlayiciUygulamasi uygulama;
    uygulama.resize(400, 300);
    uygulama.show();
    return app.exec();
}
