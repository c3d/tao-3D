#ifndef XL_HIGHLIGHTER_H
#define XL_HIGHLIGHTER_H
// ****************************************************************************
//  xl_highlighter.h                                               Tao project
// ****************************************************************************
//
//   File Description:
//
//    A syntax highlighter for XL.
//
//
//
//
//
//
//
//
// ****************************************************************************
// This software is licensed under the GNU General Public License v3.
// See file COPYING for details.
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "renderer.h"
#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QVector>
#include <set>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

namespace Tao {

class XLHighlighter : public QSyntaxHighlighter
// ----------------------------------------------------------------------------
//   XL syntax highlighter
// ----------------------------------------------------------------------------
{
    Q_OBJECT

public:
    XLHighlighter(QTextDocument *parent);

    void   highlightNames(int index, std::set<text> &set);
    void   setSelectedRanges(const XL::stream_ranges &selected);
    bool   hasSelectedObjects()    { return !selected.empty(); }
    void   clearSelectedRanges();

protected:
    void   highlightBlock(const QString &txt);

private:
    struct TextCharFormat : public QTextCharFormat
    {
        TextCharFormat()
        {
            QFont font("unifont");
            font.setPixelSize(16);
            setFont(font);
        }
    };

    struct HighlightingRule
    {
        HighlightingRule(): begin(), end(), format() {}
        HighlightingRule(const HighlightingRule &o)
            : begin(o.begin), end(o.end), format(o.format) {}
        HighlightingRule(QBrush brush, QString begin, QString end="")
            : begin(begin), end(end), format()
        {
            format.setForeground(brush);
        }
       
        QRegExp         begin;
        QRegExp         end;
        TextCharFormat  format;
    };

    bool   showSelectionInFragment(QTextFragment fragment);


    // Highlighting rules, the last one is for the names
    QVector<HighlightingRule>   highlightingRules;
    TextCharFormat              selectedFormat;
    XL::stream_ranges           selected;
};

}

#endif // XL_HIGHLIGHTER_H
