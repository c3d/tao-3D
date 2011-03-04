/**
 * @addtogroup Text Text and fonts
 * @ingroup TaoBuiltins
 *
 * Displays formatted text.
 *
 * This group has primitives to:
 *   @li Create text boxes,
 *   @li Layout text in a text box and select justification,
 *   @li Control fonts and text attributes (style, weight, underline...).
 *
 * @par About fonts and font parameters
 *
 * Tao Presentations relies on the QFont class from the Qt library to display
 * text. Therefore, the terms that are used here to describe fonts and
 * fonts attributes are the same that the ones used by Qt. You may want to
 * refer to the
 * <a href="http://doc.qt.nokia.com/latest/qfont.html">Qt documentation</a>
 * for more information.
 *
 * Example:
@verbatim
// fonts.ddd

msg := <<A quick brown fox jumps over the lazy dog.
>>
lineno := 1

write_text ->
    text text lineno & ". " & msg
    lineno := lineno + 1

text_box 0, 0, window_width - 50, window_height - 50,
    font "Times", 16
    write_text // normal
    italic
    write_text // italic
    roman
    write_text // back to normal
    bold
    write_text // bold
    regular
    write_text // back to normal
    bold
    italic
    write_text // bold and italic
    plain
    write_text // back to normal
    font "Times", 16, expanded
    write_text // stretched
    stretch 1.0
    write_text //  back to normal
    font "Lobster"
    write_text
    font "Tangerine/Bold"
    font_size 28
    write_text
@endverbatim
 *
 * Here is a screen capture:
 * @image html fonts.png "Changing fonts and font attributes (fonts.ddd)"
 *
 * @par Text layout
 *
 * You can control the horizontal and vertical distribution of text with two
 * powerful primitives: @ref align and @ref vertical_align.
 *
 * Example:
@verbatim
// justification.ddd

lorem := <<Lorem ipsum dolor sit amet, consectetur adipisicing>> &
<< elit, sed do eiusmod tempor incididunt ut labore et dolore >> &
<<magna aliqua.

Ut enim ad minim veniam, quis nostrud exercitation ullamco >> &
<<laboris nisi ut aliquip ex ea commodo consequat.>>

x0 := -135
x := x0
y := 120
w := 125
h := 230

box align ->
    locally
        color 0, 0, 1, 5%
        line_width 0
        rectangle x, y, w, h
    text_box x, y, w, h,
        font "Ubuntu", 12
        align
        text lorem
    x += w + 10

box { align_left ; vertical_align_top }
box { align_center ; vertical_align_center }
box { align_right ; vertical_align_bottom }
y -= h + 10
x := x0
box { align_justify ; vertical_align_top }
box { align_right_justify ; vertical_align_top  }
box { align_full_justify ; vertical_align_top }
@endverbatim
 *
 * @image html justification.png "Various text justifications (justification.ddd)"
 *
 * @bug [#325]
 * On MacOSX, some fonts cannot be rendered, due to
 * <a href="http://bugreports.qt.nokia.com/browse/QTBUG-11145">QTBUG-11145</a>.
 * The correction of this bug expected in QT 4.8.
 * @bug [#325] On MacOSX, font styles other than "Normal", "Bold", "Italic" and
 * "Bold Italic" cannot be selected. This is caused by
 * <a href="http://bugreports.qt.nokia.com/browse/QTBUG-13518">QTBUG-13518</a>.
 *
 * @{
 */

/**
 * Draws a text box.
 *
 * The text box is centered at @a x, @a y. Th width is @a w and
 * the height is @a h. @a contents is a block of code in which you describe
 * the contents of the box.
 */
tree text_box(real x, real y, real w, real h, code contents);

/**
 * @todo explain
 */
tree text_flow(text name);

/**
 * Inserts text into a text box.
 */
tree text(text t);

/**
 * Sets the current font.
 *
 * The font parameters are given as a comma separated list of arguments, in
 * any order. Arguments are processed in the order they appear; when
 * conflicting arguments are given, the last one takes precedence.
 *
 * For instance you may write the following expression to select
 * font family @a Ubuntu in 24 point size and bold weight:
@verbatim
font "Ubuntu", 24, bold
@endverbatim
 * The following arguments are recognized:
 * <ul>
 *   <li> @b "FontFamily" or @b "FontFamily/Style". Selects the font
 *       family (such as: Times, Arial...), and, optionally, the font style
 *       among those that are available for this font family. When a style is
 *       specified, it must be separated form the family name by a forward
 *       slash. Example styles are: "Light", "Light Italic", "Bold", "Oblique",
 *       "Demi".
 *   <li> @a number. Sets the font size, in points.
 *   <li> @b plain, @b default, @b normal. Resets all font attributes.
 *       See @ref plain.
 *   <li> Font style arguments:
 *   <ul>
 *     <li> @b roman, @b no_italic. Resets the font style. Same as @ref roman.
 *     <li> @b italic. Sets the style of the font to italic.
 *          Same as @ref italic.
 *     <li> @b oblique. Sets the style of the font to oblique.
 *         Same as @ref oblique.
 *   </ul>
 *   <li> Font weight (see also @ref weight):
 *   <ul>
 *     <li> @b light. Same as @ref light.
 *     <li> @b regular, @b no_bold. Same as @ref regular.
 *     <li> @b demibold. Sets the weight of the font an intermediate value
 *         between normal and bold.
 *     <li> @b bold. Same as @ref bold.
 *     <li> @b black. Sets the weight of the font to an intermediate value
 *          between bold and the maximum font weight.
 *   </ul>
 *   <li> Glyph stretch factor (see also @ref stretch):
 *   <ul>
 *     <li> @b ultra_condensed
 *     <li> @b extra_condensed
 *     <li> @b condensed
 *     <li> @b semi_condensed
 *     <li> @b unstreched, @b no_stretch
 *     <li> @b semi_expanded
 *     <li> @b expanded
 *     <li> @b extra_expanded
 *     <li> @b ultra_expanded
 *   </ul>
 *   <li> @b underline, @b underlined. Sets underline on. See @ref underline.
 *   <li> @b overline. Sets overline on. See @ref overline.
 *   <li> @b strike_out. Sets strikeout on. See @ref strikeout.
 *   <li> @b kerning. Sets font kerning on.
 *   <li> @b no_kerning. Sets font kerning off.
 *   <li> @b size @a s. Sets size to @a s. See @ref font_size.
 *   <li> @b slant @a s. Sets slant to @a s. See @ref slant.
 *   <li> @b weight @a w. Sets weight to @a w. See @ref weight.
 *   <li> @b stretch @a s. Sets character stretch factor to @a s.
 *       See @ref stretch.
 * </ul>
 *
 * @note You cannot provide the family string or the size in variables. The
 * following code, for instance, is invalid and will result in an
 * <em>Unexpected font keyword</em> error:
@verbatim
f := "Times"
font f, 18, bold
@endverbatim
 * Instead, you should use the specific primitives, as follows:
@verbatim
f := "Times"
font f
font_size 18
bold
@endverbatim
 *
 * When an exact match for the requested font cannot be found, the program
 * will select the closest match or possibly the application's default font.
 *
 */
tree font(code desc);

/**
 * Sets the current font.
 *
 * @a name is the font family (for instance, "Times") and optionally the font
 * style after a slash ("Times/Bold", "Futura/Medium").
 */
tree font(text name);

/**
 * Sets the current font size.
 *
 * @a size is in points (pt).
 */
tree font_size(integer size);

/**
 * Resets all the font attributes of the current font.
 *
 * Same as calling @ref roman, @ref regular, @ref underline 0,
 * @ref stretch 1.0, @ref overline 0, and @ref strikeout 0.
 */
tree plain();

/**
 * Sets the style of the current font.
 *
 * Valid values are 0 for roman, 1 for italic and 2 for oblique.
 *
 * @todo Check valid values are indeed 0, 1, and 2.
 * @bug In reality, takes a real
 * @todo Remove? (and also slant parameter in @ref font)
 */
tree slant(integer s);


/**
 * Sets the style of the current font to normal.
 *
 * Cancels italic or oblique style.
 */
tree roman();

/**
 * Sets the style of the current font to italic.
 */
tree italic();

/**
 * Sets the style of the current font to oblique.
 *
 * Oblique glyphs have an italic appearance, but are typically based
 * on the unstyled glyphs, which are slanted.
 */
tree oblique();

/**
 * Sets the weight of the current font.
 *
 * @a w should be an integer value between 0 and 99. 0 is ultralight, 99 is
 * extremely black. The following predefined values are used by
 * the @ref font function:
 *   @li @b light: 25
 *   @li @b regular, no_bold: 50
 *   @li @b demi_bold: 63
 *   @li @b bold: 75
 *   @li @b black: 87
 *
 * @bug type should be integer
 */
tree weight(real w);


/**
 * Sets the weight of the current font to normal.
 *
 * @see weight
 */
tree regular();

/**
 * Sets the weight of the current font to light.
 *
 * @see weight
 */
tree light();

/**
 * Sets the weight of the current font to bold.
 *
 * @see weight
 */
tree bold();

/**
 * Enables or disables underline.
 *
 * Set @a w to 0 to disable the attribute, or to any positive value to enable
 * it.
 */
tree underline(real w);

/**
 * Enables or disables overline.
 *
 * Set @a w to 0 to disable the attribute, or to any positive value to enable
 * it.
 */
tree overline(real w);

/**
 * Enables or disables strikeout.
 *
 * Set @a w to 0 to disable the attribute, or to any positive value to enable
 * it.
 */
tree strikeout(real w);

/**
 * Sets the stretch factor for the current font.
 *
 * For example, setting @a s to 1.5 results in all glyphs in the font
 * being 1.5 times wider. The following predefined values are used by
 * the @ref font function:
 *   @li @b ultra_condensed: 0.5
 *   @li @b extra_condensed: 0.62
 *   @li @b condensed: 0.75
 *   @li @b semi_condensed: 0.87
 *   @li @b unstretched, @b no_stretch: 1.0
 *   @li @b semi_expanded: 1.12
 *   @li @b expanded: 1.25
 *   @li @b extra_expanded: 1.5
 *   @li @b ultra_expanded: 2.0
 */
tree stretch(real s);

/**
 * Sets parameters for horizontal text layout.
 *
 * This primitive controls how words and glyphs are positioned horizontally
 * to form a line of text in the current text box.
 * The four parameters @a center, @a justify, @a spread
 * and @a full_justify may take any value between 0 and 1 (inclusive). Some
 * combinations have a usual meaning; see the functions listed below
 * for details.
 *
 * @todo The default horizontal alignment is ???.
 * @see align_left, align_right, align_center, align_justify,
 * align_right_justify, align_full_justify, align_spread, align_full_spread.
 */
tree align(real center, real justify, real spread, real full_justify);

/**
 * Align text to the left of the text box.
 * Synonym for @ref align 0, 0, 0, 0.
 */
tree align_left();

/**
 * Align text to the right of the text box.
 * Synonym for @ref align 1, 0, 0, 0.
 */
tree align_right();

/**
 * Center text horizontally in the text box.
 * Synonym for @ref align 0.5, 0, 0, 0.
 */
tree align_center();

/**
 * Justify text horizontally in the text box.
 * Text is aligned both to the left and to the right of the text box,
 * except for the last line of each paragraph, which is aligned to the left.
 * Justification is obtained by inserting extra space between words, and
 * without changing the space between characters.
 * Synonym for @ref align 0, 1, 0, 0.
 */
tree align_justify();

/**
 * Justify text horizontally in the text box.
 * Synonym for @ref align_justify.
 */
tree align_left_justify();

/**
 * Justify text horizontally in the text box.
 * Text is aligned both to the left and to the right of the text box,
 * except for the last line of each paragraph, which is aligned to the right.
 * Justification is obtained by inserting extra space between words, and
 * without changing the space between characters.
 * Synonym for @ref align 1, 1, 0, 0.
 */
tree align_right_justify();

/**
 * Justify text horizontally in the text box.
 * Text is aligned both to the left and to the right of the text box,
 * including the last line of each paragraph, even if this line is too short
 * to fit nicely in the horizontal space. In the extreme case where the last
 * line contains a single word, this word is aligned to the left.
 * Justification is obtained by inserting extra space between words, and
 * without changing the space between characters.
 * Synonym for @ref align 0, 1, 0, 1.
 */
tree align_full_justify();

/**
 * Justify characters horizontally in the text box.
 * Similar to @ref align_justify, except that extra space in each line is
 * distributed evenly between characters, not just between words.
 * The last line of each paragraph is rendered aligned the left.
 * Synonym for @ref align 0, 1, 1, 0.
 */
tree align_spread();

/**
 * Justify characters horizontally in the text box.
 * Like @ref align_spread, but all lines are fully justified (including the
 * last line of each paragraph).
 * Synonym for @ref align 0, 1, 1, 1.
 */
tree align_full_spread();

/**
 * Sets parameters for vertical text layout.
 *
 * This primitive controls how lines of text are positioned vertically in the
 * current text box. The four parameters @a center, @a justify, @a spread
 * and @a full_justify may take any value between 0 and 1 (inclusive). Some
 * combinations have a usual meaning; see the functions listed below
 * for details.
 *
 * This function is to be called once per text box, before any text is written.
 * Otherwise, unexpected results may occur such as overlapping text or
 * otherwise incorrect layout.
 *
 * The default vertical text layout is vertical_align_top.
 *
 * @see vertical_align_top, vertical_align_bottom, vertical_align_center,
 * vertical_align_justify, vertical_align_spread.
 */
tree vertical_align(real center, real justify, real spread, real full_justify);

/**
 * Flush all lines of text to the top of the text box.
 * If there are not enough lines of text to fill the box vertically, empty
 * space is at the bottom of the box.
 * Synonym for @ref vertical_align 0, 0, 0, 0.
 */
tree vertical_align_top();

/**
 * Flush all lines of text to the bottom of the text box.
 * If there are not enough lines of text to fill the box vertically, empty
 * space is at the top of the box.
 * Synonym for @ref vertical_align 1, 0, 0, 0.
 */
tree vertical_align_bottom();


/**
 * Center lines of text vertically within the text box.
 * If there are not enough lines of text to fill the box vertically, empty
 * space is equally distributed between top and bottom of the box.
 * Synonym for @ref vertical_align 0.5, 0, 0, 0.
 */
tree vertical_align_center();

/**
 * @}
 */
