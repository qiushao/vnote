#include <QtTest>

#include <models/outlinemodel.h>
#include <widgets/outlineprovider.h>

namespace tests {

class TestOutlineModel : public QObject {
  Q_OBJECT

private slots:
  void testOutlineNeverDisplaysSectionNumbers();
};

void TestOutlineModel::testOutlineNeverDisplaysSectionNumbers() {
  vnotex::OutlineModel model;

  auto outline = QSharedPointer<vnotex::Outline>::create();
  outline->m_headings.append(vnotex::Outline::Heading(QStringLiteral("First"), 1));
  outline->m_headings.append(vnotex::Outline::Heading(QStringLiteral("Second"), 2));
  outline->m_sectionNumberBaseLevel = 1;
  outline->m_sectionNumberEndingDot = true;

  model.setOutline(outline);

  const auto firstIdx = model.index(0, 0);
  QVERIFY(firstIdx.isValid());
  QCOMPARE(model.data(firstIdx, Qt::DisplayRole).toString(), QStringLiteral("First"));

  const auto secondIdx = model.index(0, 0, firstIdx);
  QVERIFY(secondIdx.isValid());
  QCOMPARE(model.data(secondIdx, Qt::DisplayRole).toString(), QStringLiteral("Second"));
}

} // namespace tests

QTEST_GUILESS_MAIN(tests::TestOutlineModel)
#include "test_outlinemodel.moc"
