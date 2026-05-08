#ifndef SUMMARYFILEPARSER_H
#define SUMMARYFILEPARSER_H

#include <QString>
#include <QVector>

namespace vnotex {

struct SummaryEntry {
  QString m_displayText;
  QString m_relativePath;
  int m_indentLevel = 0;
};

class SummaryFileParser {
public:
  static QVector<SummaryEntry> parse(const QString &p_summaryPath);

private:
  static void parseLine(const QString &p_line, SummaryEntry &p_entry, bool &p_matched);
};

} // namespace vnotex

#endif // SUMMARYFILEPARSER_H
