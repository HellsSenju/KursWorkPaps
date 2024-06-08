# KursWorkPaps
Курсовая работа по предмету " Проектирование и архитектура программных систем" 3 курс, 6 семестр

connect( manager, &IperfManager::iperfStarted, this, [=](bool res) mutable {
            qDebug() << "1";
            started = res;
            qDebug() << started;
});
