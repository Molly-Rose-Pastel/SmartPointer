#include <QtTest>
#include <iostream>

#include "SmartPointer.h"

using namespace std;

/* define custom class with destructor that tells us if the object
 * has been destroyed by setting the value of the second paramter to INT_MIN */
class customItem1{
public:
    customItem1(const int& value1, int &value2){
        data = value1;
        /* passing the address of value2 into destructor */
        destructor = &value2;
    }

    ~customItem1(){
        /* assigning INT_MIN to the data at the address that is in destructor */
        *destructor = INT_MIN;
    }
    int *destructor;
    int data;
};

class UnitTest : public QObject
{
    Q_OBJECT


private slots:
    void testConstructor();
    void testAssignmentOperator();
    void testMiscellaneous();

};


void UnitTest::testConstructor() {

    /*************************************************************************/
    /* SmartPointer(T *p) */
    /*************************************************************************/

    /* lambda function to temporarily create a SmartPointer and then
     * destroy it */
    auto create_and_destroy1 = [](customItem1 *p){

        SmartPointer<customItem1> smart(p);
    };

    /* case 1:
     * creating an item and then a smartpointer with that
     * item as parameter and then destroying that smartpointer
     * should also destroy the item */
    int testDestructor = INT_MAX;
    customItem1 *item0 = new customItem1(10, testDestructor);
    create_and_destroy1(item0);
    QCOMPARE(testDestructor, INT_MIN);

    /* case 2:
     * creating 1 item, 2 smartpointers and both take the item
     * and then destroying 1 smartpointer should NOT destroy the item */
    testDestructor = INT_MAX;
    item0 = new customItem1(20, testDestructor);
    SmartPointer<customItem1> smart0(item0);
    create_and_destroy1(item0);
    QCOMPARE(testDestructor, INT_MAX);

    /* case 3:
     * creating 2 items and 2 smartpointers , taking one item each,
     * and then destroying one of the smartpointers should destroy the
     * corresponding item but not the other */
    testDestructor = INT_MAX;
    item0 = new customItem1(30, testDestructor);
    int _testDestructor = INT_MAX;
    customItem1 *_item = new customItem1(30, _testDestructor);
    SmartPointer<customItem1> smart1(item0);
    create_and_destroy1(_item);
    QCOMPARE(testDestructor, INT_MAX);
    QCOMPARE(_testDestructor, INT_MIN);


    /*************************************************************************/
    /* SmartPointer(const SmartPointer &value) */
    /*************************************************************************/

    /* lambda function to temporarily create a SmartPointer and then
     * destroy it */
    auto create_and_destroy2 = [](const SmartPointer<customItem1> &value){

        SmartPointer<customItem1> smart(value);
    };

    auto create_and_destroy3 = [](customItem1 *p){

        SmartPointer<customItem1> smart(p);
        SmartPointer<customItem1> _smart(smart);
    };

    /* case1:
     * creating an item, then a smartpointer with that item and another
     * smartpointer with the first smartpointer and destroying both smartpointers
     * should also detroy the item */
    testDestructor = INT_MAX;
    item0 = new customItem1(10, testDestructor);
    create_and_destroy3(item0);
    QCOMPARE(testDestructor, INT_MIN);

    /* case2:
     * creating and item and then a smartpointer with that
     * item as parameter and then creating another smartpointer
     * with the previously created smartpointer as paramter
     * and then destroying that smartpointer should NOT destroy
     * the original item */
    testDestructor = INT_MAX;
    item0 = new customItem1(20, testDestructor);
    SmartPointer<customItem1> smart2(item0);
    create_and_destroy2(smart2);
    QCOMPARE(testDestructor, INT_MAX);

    /* case 3:
     * creating 2 items and 2 pairs of smartpointers , taking one item each,
     * and then destroying one of the smartpointer pairs should destroy the
     * corresponding item but not the other */
    testDestructor = INT_MAX;
    _testDestructor = INT_MAX;
    item0 = new customItem1(30, testDestructor);
    _item = new customItem1(30, _testDestructor);
    SmartPointer<customItem1> smart3(item0);
    SmartPointer<customItem1> smart4(smart3);
    create_and_destroy3(_item);
    QCOMPARE(testDestructor, INT_MAX);
    QCOMPARE(_testDestructor, INT_MIN);

}

void UnitTest::testAssignmentOperator() {

    /*************************************************************************/
    /* void operator=(T *p) */
    /*************************************************************************/

    /* lambda function to temporarily create a SmartPointer and then
     * destroy it */
    auto create_and_destroy1 = [](customItem1* value){

        SmartPointer<customItem1> smart;
        smart = value;
    };

    /* case1:
     * create 1 object and 1 smartpointer, assign the object to the smartpointer
     * then destroy the smartpointer, should destroy the object. */
    int destructor = INT_MAX;
    customItem1 * item0 = new customItem1(1, destructor);
    create_and_destroy1(item0);
    QCOMPARE(destructor, INT_MIN);

    /* case2:
     * create 1 object and 2 smartPointer, assign both to the object
     * destroy 1 smartpointer should not destroy the object. */
    destructor = INT_MAX;
    customItem1 * item1 = new customItem1(2, destructor);
    SmartPointer<customItem1> smart0;
    smart0 = item1;
    create_and_destroy1(item1);
    QCOMPARE(destructor, INT_MAX);

    /* case3:
     * create 2 objects and 2 smartpointer, assign one object to each smartpointer
     * destroy 1 smartpointer should destory the corresponding item but not
     * the other */
    destructor = INT_MAX;
    customItem1 *item2 = new customItem1(3, destructor);
    SmartPointer<customItem1> smart1;
    smart1 = item2;
    int _destructor = INT_MAX;
    customItem1 *item3 = new customItem1(3, _destructor);
    create_and_destroy1(item3);
    QCOMPARE(destructor, INT_MAX);
    QCOMPARE(_destructor, INT_MIN);

    /* case4:
     * create 2objects and 1 smartpointer, assign the smartpointer to the first
     * object, then assign it to the other object, should destroy the first one. */
    destructor = INT_MAX;
    _destructor = INT_MAX;
    customItem1 *item4 = new customItem1(4, destructor);
    customItem1 *item5 = new customItem1(4, _destructor);
    SmartPointer<customItem1> smart2;
    smart1 = item4;
    smart1 = item5;
    QCOMPARE(destructor, INT_MIN);
    QCOMPARE(_destructor, INT_MAX);

    /* case5:
     * create 1 object, assign smartpointer, then reassign smartpointer to same
     * object should not destroy the object */
    destructor = INT_MAX;
    customItem1 *item6 = new customItem1(5, destructor);
    SmartPointer<customItem1> smart3;
    smart3 = item6;
    smart3 = item6;
    QCOMPARE(destructor, INT_MAX);


    /*************************************************************************/
    /* void operator=(const SmartPointer &value) */
    /*************************************************************************/

    /* lambda function to temporarily create a SmartPointer and then
     * destroy it */
    auto create_and_destroy2 = [](const SmartPointer<customItem1>& value){

        SmartPointer<customItem1> smart;
        smart = value;
    };

    auto create_and_destroy3 = [](customItem1 *value){

        SmartPointer<customItem1> smart;
        smart = value;
        SmartPointer<customItem1> _smart;
        _smart = smart;
    };

    /* case1:
     * create object, assign it to sp1, assign sp1 to sp2, destroy one
     * of the sp should not destroy the object */
    destructor = INT_MAX;
    customItem1 *item7 = new customItem1(1, destructor);
    SmartPointer<customItem1> smart4 = item7;
    create_and_destroy2(smart4);
    QCOMPARE(destructor, INT_MAX);

    /* case2:
     * create object, assing to sp1, assign sp1 to sp2, destroy both, should
     * also destroy object */
    destructor = INT_MAX;
    customItem1 *item8 = new customItem1(2, destructor);
    create_and_destroy3(item8);
    QCOMPARE(destructor, INT_MIN);

    /* case3:
     * create 2 objects and 2 pairs of sp, chain-assign each sp pair and destroy
     * one pair, should destroy the corresponding item but not the other one. */
    destructor = INT_MAX;
    _destructor = INT_MAX;
    customItem1 *item9 = new customItem1(3, destructor);
    customItem1 *item10 = new customItem1(3, _destructor);
    SmartPointer<customItem1> smart5 = item9;
    SmartPointer<customItem1> smart6 = smart5;
    create_and_destroy3(item10);
    QCOMPARE(destructor, INT_MAX);
    QCOMPARE(_destructor, INT_MIN);

    /* case4:
     * 2 objects, 2 smartpointer, assign one to each, then reassign one sp
     * to the other, should destroy the corresponding object */
    destructor = INT_MAX;
    _destructor = INT_MAX;
    customItem1 * item11 = new customItem1(4, destructor);
    customItem1 * item12 = new customItem1(4, _destructor);
    SmartPointer<customItem1> smart7 = item11;
    SmartPointer<customItem1> smart8 = item12;
    smart8 = smart7;
    QCOMPARE(destructor, INT_MAX);
    QCOMPARE(_destructor, INT_MIN);


    /* case5:
     * 1 object 1 smartpointer, reassign the smartpointer to itself should
     * not destroy the object. */
    destructor = INT_MAX;
    customItem1 * item13 = new customItem1(4, destructor);
    SmartPointer<customItem1> smart9 = item13;
    smart9 = smart9;
    QCOMPARE(destructor, INT_MAX);
}

void UnitTest::testMiscellaneous() {

    /*************************************************************************/
    /* T* operator->() const */
    /*************************************************************************/

    /* case 1:
     * creating object, assign smartpointer to it, operater-> should give
     * direct access to the object pointer */
    int destructor = INT_MAX;
    customItem1 * item0 = new customItem1(1, destructor);
    SmartPointer<customItem1> smart0(item0);

    QCOMPARE(item0, smart0.operator ->());

    /*************************************************************************/
    /* T& operator*() const */
    /*************************************************************************/

    /* case 1:
     * creating object, assign smarpointer to it, operator* should give the
     * value of the object */

    int *intItem = new int(5);
    SmartPointer<int> smartInt(intItem);
    QCOMPARE(*intItem, smartInt.operator *());

    /*************************************************************************/
    /* bool operator==(const SmartPointer &value) */
    /*************************************************************************/

    /* case 1:
     * create 1 object assign it to 2 smartpointer, comparing them should
     * yield 'true' */
    SmartPointer<customItem1> smart1(smart0);

    QVERIFY(smart0 == smart1);

    /* case 2:
     * create 2 objects, assign to 1 smartpointer each, comparing them should
     * yield 'false' */
    customItem1 * item1 = new customItem1(1, destructor);
    smart1 = item1;

    QCOMPARE(smart0 == smart1, false);


}

QTEST_APPLESS_MAIN(UnitTest)

#include "tst_unittest.moc"
