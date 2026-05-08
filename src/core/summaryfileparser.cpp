#include "summaryfileparser.h"

#include <QFile>
#include <QRegularExpression>
#include <QTextStream>

namespace vnotex {

void SummaryFileParser::parseLine(const QString &p_line, SummaryEntry &p_entry, bool &p_matched) {
  p_matched = false;

  // Match: optional leading whitespace, optional "- " prefix, [text](path)
  // Path may contain spaces; optional "title" inside parens is stripped.
  static const QRegularExpression re(
      R"(^(\s*)(?:-\s*)?\[([^\]]+)\]\((.+)\)\s*$)");

  auto match = re.match(p_line);
  if (!match.hasMatch()) {
    return;
  }

  p_matched = true;
  const auto indent = match.captured(1);
  p_entry.m_indentLevel = indent.length() / 2;

  p_entry.m_displayText = match.captured(2).trimmed();

  auto path = match.captured(3).trimmed();
  // Strip optional title/alt text: path "title" or path 'title'
  static const QRegularExpression titleRe(
      R"(^(.+)\s+["'][^"']*["']\s*$)");
  auto titleMatch = titleRe.match(path);
  if (titleMatch.hasMatch()) {
    path = titleMatch.captured(1).trimmed();
  }

  p_entry.m_relativePath = path;
}

QVector<SummaryEntry> SummaryFileParser::parse(const QString &p_summaryPath) {
  QVector<SummaryEntry> entries;

  QFile file(p_summaryPath);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return entries;
  }

  QTextStream in(&file);
  in.setEncoding(QStringConverter::Utf8);

  while (!in.atEnd()) {
    const auto line = in.readLine();
    if (line.isEmpty() || line.startsWith(QLatin1String("#")) ||
        line.startsWith(QLatin1String("---"))) {
      continue;
    }

    SummaryEntry entry;
    bool matched = false;
    parseLine(line, entry, matched);
    if (matched && !entry.m_relativePath.isEmpty()) {
      entries.append(entry);
    }
  }

  return entries;
}

} // namespace vnotex
