#include <QString>
#include <QtTest>

#include "geom.h"

class TestSuosuoban : public QObject
{
    Q_OBJECT

public:
    TestSuosuoban();

private Q_SLOTS:
    void testGeom();
};

TestSuosuoban::TestSuosuoban()
{
}

void TestSuosuoban::testGeom()
{
    PolyLine polyLine;

    QVERIFY(polyLine.size==0);
    QVERIFY(polyLine.head==NULL);

    VecDouble vec;
    vec.x = 1;
    vec.y = 1;

    PolyLine::NodeType* node;
    node = new PolyLine::NodeType;
    node->value = vec;
    polyLine.pushBack(node);

    QVERIFY(polyLine.size==1);
    QVERIFY(polyLine.head);
}

QTEST_APPLESS_MAIN(TestSuosuoban)

#include "tst_testsuosuoban.moc"
