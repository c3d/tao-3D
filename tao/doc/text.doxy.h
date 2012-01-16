/**
 * @~english
 * @addtogroup Text Text layout and fonts
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
 * fonts attributes are the same as those used by Qt. You may want to
 * refer to the
 * <a href="http://doc.qt.nokia.com/latest/qfont.html">Qt documentation</a>
 * for more information.
 *
 * Example (<a href="examples/fonts.ddd">fonts.ddd</a>):
 * @include fonts.ddd
 *
 * Here is a screen capture:
 * @image html fonts.png "Changing fonts and font attributes (fonts.ddd)"
 *
 * @par Fonts bundled with Tao Presentations
 *
 * Tao Presentation comes with pre-installed Open Source fonts you can use in
 * your documents. By using them, you can be sure that your presentation will
 * show correctly on someone else's computer even on a different operating
 * system (Microsoft Windows, MacOSX, Linux).
 *
 * Here is a complete list of the fonts that are packaged with Tao. Be aware
 * that family and style names are case sensitive; they must be typed as they
 * appears in the table (including spaces between words). For example:
 *
 * @~french
 * @addtogroup Text Mise en forme de texte et polices de caractères
 * @ingroup TaoBuiltins
 *
 * Affiche du texte formatté.
 *
 * Ce groupe contient de primitives pour :
 *   @li Créer des zones de texte,
 *   @li Positionner et justifier le texte,
 *   @li Choisir la police de caractère et les attributs du texte (style,
 *       graisse, soulignement...).
 *
 * @par À propos des polices de caractères
 *
 * Tao Presentations se repose sur la classe QFont de la bibliothèque Qt pour
 * afficher le texte. Par conséquent les termes utilisés ici pour décrire les
 * polices et les attributs de polices sont les mêmes que ceux utilisés par
 * Qt. Vous pouvez vous référer à
 * <a href="http://doc.qt.nokia.com/latest/qfont.html">la documentation Qt</a>
 * (en anglais).
 *
 * Voici un exemple (<a href="examples/fonts.ddd">fonts.ddd</a>) :
 * @include fonts.ddd
 *
 * Voici une capture d'écran :
 * @image html fonts.png "Utilisation des polices de caractères"
 *
 * @par Polices livrées avec Tao Presentations
 *
 * Tao Presentation est fourni avec des polices <em>Open Source</em> que vous
 * pouvez utiliser dans vos documents. En les utilisant, vous pouvez être
 * certain que vos présentations s'afficheront correctement sur un autre
 * ordinateur, même si le système n'est pas le même (Microsoft Windows, MacOSX,
 * Linux).
 *
 * Voici une liste complète des polices fournies avec Tao Presentations. Les
 * noms de famille et de style sont sensibles à la casse (majuscules et
 * minuscules). Il faut les copier telles qu'elles apparaîssent dans le
 * tableau (y compris les espaces entre les mots). Par exemple :
 * @code
 * font "Gentium Plus"
 * font "Allerta/Stencil"
 * @endcode
 *
 * @~
 * <table>
 *  <tr>
 *   <th colspan="2"><b>Serif fonts</b></th></td>
 *  </tr>
 *     <tr><th>Family</th><th>Styles</th></tr>
 *     <tr><td>Bentham</td><td>Regular</td></tr>
 *     <tr><td>Gentium Plus</td><td>Regular, Italic</td></tr>
 *     <tr><td>Goudy Bookletter 1911</td><td>Regular</td></tr>
 *     <tr><td>IM FELL English</td><td>Regular, Italic</td></tr>
 *     <tr><td>IM FELL English SC</td><td>Regular</td></tr>
 *     <tr><td>Old Standard TT</td><td>Regular, Bold, Italic</td></tr>
 *     <tr><td>PT Serif</td><td>Regular, Italic, Bold, Bold Italic</td></tr>
 *  <tr>
 *   <th colspan="2"><b>Sans-serif fonts</b></th></td>
 *  </tr>
 *     <tr><th>Family</th><th>Styles</th></tr>
 *     <tr><td>Andika Basic</td><td>Regular</td></tr>
 *     <tr><td>Cherry Cream Soda</td><td>Regular</td></tr>
 *     <tr><td>Droid Sans</td><td>Regular, Bold</td></tr>
 *     <tr><td>Droid Sans Mono</td><td>Regular</td></tr>
 *     <tr><td>Ubuntu</td><td>Regular, Italic, Bold, Bold Italic</td></tr>
 *     <tr><td>Yanone Kaffeesatz</td><td>Thin, Light, Regular, Bold</td></tr>
 *  <tr>
 *   <th colspan="2"><b>Handwriting fonts</b></th></td>
 *  </tr>
 *     <tr><th>Family</th><th>Styles</th></tr>
 *     <tr><td>Calligraffiti</td><td>Regular</td></tr>
 *     <tr><td>Coming Soon</td><td>Regular</td></tr>
 *     <tr><td>Dancing Script</td><td>Regular</td></tr>
 *     <tr><td>Homemade Apple</td><td>Regular</td></tr>
 *     <tr><td>Just Another Hand</td><td>Regular</td></tr>
 *     <tr><td>Kristi</td><td>Medium</td></tr>
 *     <tr><td>Tangerine</td><td>Regular, Bold</td></tr>
 *  <tr>
 *   <th colspan="2"><b>Miscellaneous fonts</b></th></td>
 *  </tr>
 *     <tr><th>Family</th><th>Styles</th></tr>
 *     <tr><td>Allerta</td><td>Stencil</td></tr>
 *     <tr><td>Chewy</td><td>Regular</td></tr>
 *     <tr><td>Geo</td><td>Regular</td></tr>
 *     <tr><td>Kenia</td><td>Regular</td></tr>
 *     <tr><td>Kranky</td><td>Regular</td></tr>
 *     <tr><td>LeckerliOne</td><td>Regular</td></tr>
 *     <tr><td>Lobster</td><td>Regular</td></tr>
 *     <tr><td>Luckiest Guy</td><td>Regular</td></tr>
 *     <tr><td>Mountains of Christmas</td><td>Regular</td></tr>
 *     <tr><td>Orbitron</td><td>Regular, Medium, Bold, Black</td></tr>
 *     <tr><td>Permanent Marker</td><td>Regular</td></tr>
 *     <tr><td>UnifrakturMaguntia</td><td>Book</td></tr>
 *     <tr><td>Unkempt</td><td>Regular</td></tr>
 * </table>
 *
 * @~english
 * @par Using additional fonts
 *
 * All the fonts installed on the computer are available for use by Tao. So, if
 * you need to use a specific font, you may simply install it and restart Tao
 * for the change to take effect.
 *
 * Another way of using a specific font without installing it on the computer
 * is to copy the font file (TrueType or OpenType font, usually with one of the
 * following extnsions: .ttf, .otf, .ttc) into the @c font sub-folder in the
 * document folder.
 *
 * @par Text layout
 *
 * You can control the horizontal and vertical distribution of text with two
 * powerful primitives: @ref align and @ref vertical_align.
 *
 * Example (<a href="examples/justification.ddd">justification.ddd</a>):
 * @include justification.ddd
 *
 * @image html justification.png "Various text justifications (justification.ddd)"
 *
 * @bug [#325] On MacOSX, some fonts cannot be rendered, due to
 * <a href="http://bugreports.qt.nokia.com/browse/QTBUG-11145">QTBUG-11145</a>.
 *      The correction of this bug expected in QT 4.8.
 * @bug [#325] On MacOSX, font styles other than "Normal", "Bold", "Italic" and
 *     "Bold Italic" cannot be selected. This is caused by
 * <a href="http://bugreports.qt.nokia.com/browse/QTBUG-13518">QTBUG-13518</a>.
 *
 * @todo font_scaling not documented - I don't understand its purpose
 * @todo The following primitives are not documented yet: paragraph_space,
 *       character_space (should it be called word_space ?), line_break,
 *       sentence_break, paragraph_break, column_break, page_break.
 * @todo Document enable_glyph_cache?
 *
 * @~french
 * @par Utilisation de polices de caractères additionnelles
 *
 * Toutes les polices installées sur votre ordinateur sont disponibles et
 * peuvent être utilisées par Tao Presentations. Donc, si vous avez besoin
 * d'une police de caractères précise, vous pouvez simplement l'installer et
 * redémarrer Tao Presentations pour qu'elle devienne visible.
 *
 * Une autre manière d'utiliser une police spécifique sans l'installer sur
 * votre ordinateur est de copier le fichier de police (TrueType ou OpenType,
 * typiquement avec l'extension .ttf, .otf ou .ttc) dans le sous-dossier
 * @c fonts du dossier dans lequel se trouve le document.
 *
 * @par Mise en forme du texte
 *
 * Vous pouvez contrôler la distribution horizontale et verticale du texte
 * grâce aux primitives @ref align and @ref vertical_align.
 *
 * Exemple (<a href="examples/justification.ddd">justification.ddd</a>) :
 * @include justification.ddd
 *
 * @image html justification.png "Justification de texte"
 *
 * @~
 * @{
 */

/**
 * @~english
 * Defines and names a text flow.
 *
 * Use this function to name a text flow that will then be displayed in one
 * or more text box (with @ref text_box).
 *
 * Here is the typical use:
@code
// Create and name the text flow
text_flow "First",
    text "Some long text"

// Then, later:
text_box "First", -100, 0, 200, 400
text_box "First",  100, 0, 200, 400  // Continuation of 1st text box
@endcode
 *
 * @~french
 * Definit et nomme un flot de texte.
 *
 * Cette fonction permet d'afficher du texte en plusieurs zones (avec
 * @ref text_box).
 *
 * Voici un exemple :
@code
// Crée un flot de texte nommé
text_flow "Flot1",
    text "Du texte très long"

// Puis ensuite :
text_box "Flot1", -100, 0, 200, 400
text_box "Flot1",  100, 0, 200, 400  // Suite de la première boîte de texte
@endcode
 */
text_flow(name:text, contents:tree);


/**
 * @~english
 * Draws a text box to show a text flow.
 *
 * The text box is centered at @p x, @p y. The width is @p w and
 * the height is @p h. @p name is the name of the flow to display.
 * The displayed text will be the continuation of the flow.
 *
 * @~french
 * Crée une boîte pour afficher un flot de texte.
 *
 * La boîte est centrée en (@p x, @p y). Sa largeur est @p w et
 * la hauteur est @p h. @p name est le nom du flot de texte à afficher.
 */
text_box(name:text, x:real, y:real, w:real, h:real);


/**
 * @~english
 * Draws a text box.
 *
 * The text box is centered at @p x, @p y. The width is @p w and
 * the height is @p h. @p contents is a block of code in which you describe
 * the content of the box.
 *
 * @~french
 * Crée une boîte pour afficher du texte.
 *
 * La boîte est centrée en (@p x, @p y). Sa largeur est @p w et
 * la hauteur est @p h. @p contents est un bloc de code dans lequel vous
 * décrivez le contenu de la boîte.
 */
text_box(x:real, y:real, w:real, h:real, contents:tree);


/**
 * @~english
 * Isolate text modifications.
 *
 * Color, font and other text related modification done in this container
 * are not visible from outside.
 * @warning The paragraph modification are not isolated. See @ref para_span
 *
 * @~french
 * Isole des modifications de texte.
 *
 * Les modifications de couleur, de police ou similaires qui sont faites dans
 * @p contents n'ont pas d'effet à l'extérieur du text_span.
 * @warning Les modifications de paragraphe ne sont pas concernées.
 * Cf. @ref para_span.
 */
text_span(contents:tree);

/**
 * @~english
 * Create an isolated paragraph
 *
 * Text alignment, paragraph space, font, color and other text and paragraph
 * related modifications done in this container are not visible from outside.
 *
 * @~french
 * Crée un paragraphe isolé.
 *
 * Les modifications des caractéristiques de paragraphe, telles que l'alignment
 * du texte, l'espacement des parapgraphes, les polices, la couleur, qui sont
 * faites dans @p contents n'ont pas d'effet à l'extérieur du para_span.
 */
para_span(contents:tree);


/**
 * @~english
 * Inserts text into a text box.
 * @~french
 * Ajoute du texte dans une zone de texte.
 * @~
 * @see text_box.
 */
text(t:text);

/**
 * @~english
 * Sets the current font.
 *
 * The font parameters are given as a comma separated list of arguments, in
 * any order. Arguments are processed in the order they appear; when
 * conflicting arguments are given, the last one takes precedence.
 *
 * For instance you may write the following expression to select
 * font family @p Ubuntu in 24 point size and bold weight:
 * @code
 * font "Ubuntu", 24, bold
 * @endcode
 * The following arguments are recognized:
 * <ul>
 *   <li> @b "FontFamily" or @b "FontFamily/Style". Selects the font
 *       family (such as: Times, Arial...), and, optionally, the font style
 *       among those that are available for this font family. When a style is
 *       specified, it must be separated form the family name by a forward
 *       slash. Example styles are: "Light", "Light Italic", "Bold", "Oblique",
 *       "Demi". @n Some style names are equivalent to style primitives or
 *       keywords: "Bold" is equivalent to @ref bold, "Italic" is equivalent to
 *       @ref italic. "Regular" is equivalent to @ref roman. For instance:
 * @code
 * font "Ubuntu/Bold Italic"
 * @endcode
 *       has the same effect as:
 * @code
 * font "Ubuntu"
 * bold
 * italic
 * @endcode
 *        Consequently, the effect of the style qualifier applies not only
 *        to the current font, but also to the subsequent font declarations.
 *        In the following example, @a both words are italicized:
 * @code
 * text_box 0, 0, 200, 100,
 *     font "Andika Basic/Italic"
 *     text "Hello, "
 *     font "Andika Basic"  // does NOT cancel italic
 *     text "world!"
 * @endcode
 *        To cancel the italic mode, you would need to call @ref roman or
 *        use the "/Regular" style qualifier, like this:
 * @code
 * text_box 0, 0, 200, 100,
 *     font "Andika Basic/Italic"
 *     text "Hello, "
 *     font "Andika Basic/Regular"  // cancels italic
 *     text "world!"
 * @endcode
 *   <li> @p number. Sets the font size, in points.
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
 *   <li> @b size @p s. Sets size to @p s. See @ref font_size.
 *   <li> @b slant @p s. Sets slant to @p s. See @ref slant.
 *   <li> @b weight @p w. Sets weight to @p w. See @ref weight.
 *   <li> @b stretch @p s. Sets character stretch factor to @p s.
 *       See @ref stretch.
 *   <li> Capitalization:
 *   <ul>
 *     <li> @b mixed_case, @b normal_case. Resets capitalization mode to
 *          the normal, mixed case.
 *     <li> @b uppercase. Render text in upper case type.
 *     <li> @b lowercase. Render text in lower case type.
 *     <li> @b small_caps. Render text in small-caps type.
 *     <li> @b capitalized. Render the first character of each word as an
 *       uppercase character.
 *   </ul>
 * </ul>
 *
 * @note You cannot provide the family string or the size in variables. The
 * following code, for instance, is invalid and will result in an
 * <em>Unexpected font keyword</em> error:
@code
f := "Times"
font f, 18, bold
@endcode
 * Instead, you should use the specific primitives, as follows:
@code
f := "Times"
font f
font_size 18
bold
@endcode
 *
 * When an exact match for the requested font cannot be found, the program
 * will select the closest match or possibly the application's default font.
 *
 * @~french
 * Sélectionne la police de caractère courante.
 *
 * Les paramètres de police de caractères sont donnés sous forme d'une liste
 * de paramètres séparés par des virgules, dans un ordre quelconque. Ces
 * paramètres sont traités dans l'ordre où ils sont donnés. Lorsque des valeurs
 * contradictoires sont données, seule la dernière valeur est conservée.
 *
 * Par exemple, pour sélectionner la police de la famille @p Ubuntu en taille
 * 24 points et en gras :
 * @code
 * font "Ubuntu", 24, bold
 * @endcode
 * Les paramètres suivants sont reconnus :
 * <ul>
 *   <li> @b "FamilleDeCaractères" ou @b "FamilleDeCaractères/Style".
 *       Sélectionne la famille de caractères
 *       (comme : Times, Arial...), et, de manière optionnelle, le style parmi
 *       ceux qui sont disponibles pour cette famille. Quand un style est
 *       spécifié, il doit être séparé du nom de la famille par un slash
 *       (@c /). Des exemples de styles : "Light", "Light Italic", "Bold",
 *       "Oblique", "Demi". @n Certains noms de styles sont les mêmes que les
 *       primitives qui ont le même effet :
 *       "Bold" est équivalent à @ref bold, "Italic" est équivalent à
 *       @ref italic. "Regular" est équivalent à @ref roman. Par exemple :
 * @code
 * font "Ubuntu/Bold Italic"
 * @endcode
 *       a le même effet que:
 * @code
 * font "Ubuntu"
 * bold
 * italic
 * @endcode
 *        Par conséquent, l'effet du qualificatif de style s'applique non
 *        seulement à la police courant, mais aussi aux déclarations suivantes.
 *        Dans l'exemple qui suit, @a les @a deux mots sont en italique :
 * @code
 * text_box 0, 0, 200, 100,
 *     font "Andika Basic/Italic"
 *     text "Andika "
 *     font "Andika Basic"  // n'annule PAS l'italique
 *     text "Basic"
 * @endcode
 *        Pour annuler l'italique, il faut utiliser @ref roman ou le
 *        qualificatif "/Regular" comme ceci :
 * @code
 * text_box 0, 0, 200, 100,
 *     font "Andika Basic/Italic"
 *     text "Andika "
 *     font "Andika Basic/Regular"  // annule l'italique
 *     text "Basic"
 * @endcode
 *   <li> @p un nombre. Définit la taille de la police, en points.
 *   <li> @b plain, @b default, @b normal. Ré-initialise tous les attributs.
 *       Cf. @ref plain.
 *   <li> Les paramètres de style :
 *   <ul>
 *     <li> @b roman, @b no_italic. Ré-initialise le style. Cf. @ref roman.
 *     <li> @b italic. Sélectionne l'italique.
 *          Cf. @ref italic.
 *     <li> @b oblique. Sélectionne l'oblique.
 *          Cf. @ref oblique.
 *   </ul>
 *   <li> Graisse du caractère (cf. @ref weight):
 *   <ul>
 *     <li> @b light. Cf. @ref light.
 *     <li> @b regular, @b no_bold. Cf. @ref regular.
 *     <li> @b demibold. Sélectionne une graisse intermédiaire entre @b regular
 *             et @b bold.
 *     <li> @b bold. Cf. @ref bold.
 *     <li> @b black. Sélectionne une graisse intermédiaire entre @b bold
 *             et la graisse maximale pour cette police.
 *   </ul>
 *   <li> Le facteur d'étirement des glyphs (cf. @ref stretch):
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
 *   <li> @b underline, @b underlined. Active le soulignement.
 *        Cf. @ref underline.
 *   <li> @b overline. Trace une ligne au-dessus du texte. Cf. @ref overline.
 *   <li> @b strike_out. Barre le texte. Cf. @ref strikeout.
 *   <li> @b kerning. Active le kerning.
 *   <li> @b no_kerning. Désactive le kerning.
 *   <li> @b size @p s. Sélectionne la taille : @p s points.
 *        Cf. @ref font_size.
 *   <li> @b slant @p s. Sélectionne l'inclinaison @p s. Cf. @ref slant.
 *   <li> @b weight @p w. Sélectionne la graisse @p w. Cf. @ref weight.
 *   <li> @b stretch @p s. Sélectionne le facteur d'étirement @p s.
 *        Cf. @ref stretch.
 *   <li> Casse:
 *   <ul>
 *     <li> @b mixed_case, @b normal_case. Ré-initialise le mode de
 *          capitalisation (majuscules et minuscules).
 *     <li> @b uppercase. Affiche le texte en majuscules.
 *     <li> @b lowercase. Affiche le texte en minuscules.
 *     <li> @b small_caps. Affiche le texte en petite capitales.
 *     <li> @b capitalized. Affiche le premier caractère de chaque mot en
 *          capitale.
 *   </ul>
 * </ul>
 *
 * @note Vous ne pouvez pas passer le nom de famille de caractères, ni la
 * taille. Le code suivant, par exemple, est incorrect et cause une erreur
 * <em>Unexpected font keyword</em> error:
@code
f := "Times"
font f, 18, bold
@endcode
 * La façon correcte est :
@code
f := "Times"
font f
font_size 18
bold
@endcode
 *
 * Lorsque la police de caractères exacte n'est pas trouvée, le programme
 * sélectionne la police la plus proche ou éventuellement la police par
 * défaut.
 */
font(desc:tree);

/**
 * @~english
 * Sets the current font.
 *
 * @p name is the font family (for instance, "Times") and optionally the font
 * style after a slash ("Times/Bold", "Futura/Medium").
 *
 * @~french
 * Sélectionne la police de caractère courante.
 *
 * @p name est le nom de la famille de caractères (par exemple, "Times") et
 * éventuellement un style après un slash ("Times/Bold", "Futura/Medium").
 */
font(name:text);

/**
 * @~english
 * Sets the current font size.
 * @p size is in points (pt).
 * @~french
 * Sélectionne la taille des caractères.
 * @p size est exprimé en points (pt).
 */
font_size(size:real);

/**
 * @~english
 * Resets all the font attributes of the current font.
 * Same as the following code:
 * @~french
 * Ré-initialise les attributs de la police de caractères courante.
 * Équivalent au code suivant :
 * @~
 * @code
roman
regular
underline 0
stretch 1.0
overline 0
strikeout 0
 * @endcode
 */
plain();

/**
 * @~english
 * Sets the style of the current font.
 *
 * Valid values are 0 for roman, 1 for italic and 2 for oblique.
 *
 * - normal	0	Normal glyphs used in unstyled text.
 * - italic	1	Italic glyphs that are specifically designed for the purpose of representing italicized text.
 * - oblique	2	Glyphs with an italic appearance that are typically based on the unstyled glyphs, but are not fine-tuned for the purpose of representing italicized text.
 * @~french
 * Sélectionne le style de la police de caractères courante.
 *
 * Les valeurs possible de @p s sont 0 pour normal, 1 pour italique et 2 pour oblique.
 * @~
 * @see roman, italic, oblique
 */
slant(s:real);


/**
 * @~english
 * Sets the style of the current font to normal.
 * Cancels italic or oblique style.
 * @~french
 * Sélectionne le style de caractères normal.
 * Annule l'italique et l'oblique.
 */
roman();

/**
 * @~english
 * Sets the style of the current font to italic.
 * @~french
 * Sélectionne le style de caractères italique.
 */
italic();

/**
 * @~english
 * Sets the style of the current font to oblique.
 * Oblique glyphs have an italic appearance, but are typically based
 * on the unstyled glyphs, which are slanted.
 * @~french
 * Sélectionne le style de caractères italique.
 * L'oblique a une apparence penchée similaire à l'italique mais les glyphes
 * sont basés sur les formes normales et sont inclinés. Les glyphs et ne sont pas
 * spécialement dessinés pour l'italique.
 */
oblique();

/**
 * @~english
 * Sets the weight of the current font.
 *
 * @p w should be an integer value between 0 and 99. 0 is ultralight, 99 is
 * extremely black. The following predefined values are used by
 * the @ref font primitive:
 *
 * @~french
 * Sélectionne la graisse de la police de caractères courante.
 * @p w est un entier entre 0 et 99. 0 est une graisse extrêmement faible,
 * 99 est extrêmement gras. Les valeurs suivantes sont utilisées par la
 * primitive @ref font :
 * @~
 *   @li @b light: 25quivale
 *   @li @b regular, no_bold: 50
 *   @li @b demi_bold: 63
 *   @li @b bold: 75
 *   @li @b black: 87
 */
weight(w:real);


/**
 * @~english
 * Sets the weight of the current font to normal.
 * @~french
 * Sélectionne la graisse normale.
 * @~
 * @see weight
 */
regular();

/**
 * @~english
 * Sets the weight of the current font to light.
 * @~french
 * Sélectionne la graisse fine.
 * @~
 * @see weight
 */
light();

/**
 * @~english
 * Sets the weight of the current font to bold.
 * @~french
 * Sélectionne la graisse grasse.
 * @~
 * @see weight
 */
bold();

/**
 * @~english
 * Enables or disables underline.
 * Set @p w to 0 to disable the attribute, or to any positive value to enable
 * it.
 * @~french
 * Active ou désactive le soulignement.
 * Le soulignement est désactivé si @p w vaut 0 et activé sinon.
 */
underline(w:real);

/**
 * @~english
 * Enables or disables overline.
 * Set @p w to 0 to disable the attribute, or to any positive value to enable
 * it.
 * @~french
 * Active ou désactive le surlignement.
 * Le surlignement est désactivé si @p w vaut 0 et activé sinon.
 * Le surlignement est une ligne au-dessus du texte.
 */
overline(w:real);

/**
 * @~english
 * Enables or disables strikeout.
 * Set @p w to 0 to disable the attribute, or to any positive value to enable
 * it.
 * @~french
 * Active ou désactive le mode barré.
 * Le barré est désactivé si @p w vaut 0 et activé sinon.
 */
strikeout(w:real);

/**
 * @~english
 * Sets the stretch factor for the current font.
 *
 * For example, setting @p s to 1.5 results in all glyphs in the font
 * being 1.5 times wider. The following predefined values are used by
 * the @ref font function:
 * @~french
 * Sélectionne le facteur d'étirement de la police de caractères courante.
 * La largeur des caractères est modifiée du facteur @p s.
 * Les valeurs suivantes sont utilisées par la primitive @ref font :
 * @~
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
stretch(s:real);

/**
 * @~english
 * Sets parameters for horizontal text layout.
 *
 * This primitive controls how words and glyphs are positioned horizontally
 * to form a line of text in the current text box.
 * The four parameters @p center, @p justify, @p spread
 * and @p full_justify may take any value between 0 and 1 (inclusive). Some
 * combinations have a usual meaning; see the functions listed below
 * for details.
 *
 * Default alignment is @ref align_left.
 * @see align_left, align_right, align_center, align_justify,
 * align_right_justify, align_full_justify, align_spread, align_full_spread.
 *
 * @~french
 * Définit les paramètres de justification horizontale.
 *
 * Cette primitive contrôle comment les mots et les caractères sont
 * positionnés horizontalement pour former une ligne.
 * Les quatre parametres @p center, @p justify, @p spread
 * et @p full_justify sont entre 0 and 1 (inclus). Certaines combinaisons
 * ont une signification usuelle, cf. les fonctions listées ci-dessous pour
 * plus de détails.
 *
 * L'alignement horizontal par défaut est @ref align_left.
 *
 * @~
 * @see align_left, align_right, align_center, align_justify,
 * align_right_justify, align_full_justify, align_spread, align_full_spread.
 */
align(center:real, justify:real, spread:real, full_justify:real);

/**
 * @~english
 * Sets parameters for horizontal text layout.
 * This is equivalent to:
 *
 * @~french
 * Définit les paramètres de justification horizontale.
 * Équivalent à :
 *
 * @~
 * @code align center, justify, spread, 0 @endcode
 */
align(center:real, justify:real, spread:real);

/**
 * @~english
 * Sets parameters for horizontal text layout.
 * This is equivalent to:
 *
 * @~french
 * Définit les paramètres de justification horizontale.
 * Équivalent à :
 *
 * @~
 * @code align center, justify, 0, 0 @endcode
 */
align(center:real, justify:real);

/**
 * @~english
 * Sets parameters for horizontal text layout.
 * This is equivalent to:
 *
 * @~french
 * Définit les paramètres de justification horizontale.
 * Équivalent à :
 *
 * @~
 * @code align center, 0, 0, 0 @endcode
 */
align(center:real);

/**
 * @~english
 * Align text to the left of the text box.
 * This is equivalent to:
 * @~french
 * Aligne le texte à gauche.
 * Équivalent à :
 * @~
 * @code align 0, 0, 0, 0 @endcode
 */
align_left();

/**
 * @~english
 * Align text to the right of the text box.
 * This is equivalent to:
 * @~french
 * Aligne le texte à droite.
 * Équivalent à :
 * @~
 * @code align 1, 0, 0, 0 @endcode
 */
align_right();

/**
 * @~english
 * Center text horizontally in the text box.
 * This is equivalent to:
 * @~french
 * Centre le texte horizontalement.
 * Équivalent à :
 * @~
 * @code align 0.5, 0, 0, 0 @endcode
 */
align_center();

/**
 * @~english
 * Justify text horizontally in the text box.
 * Text is aligned both to the left and to the right of the text box,
 * except for the last line of each paragraph, which is aligned to the left.
 * Justification is obtained by inserting extra space between words, and
 * without changing the space between characters.
 * This is equivalent to:
 * @~french
 * Justifie le texte horizontalement à gauche et à droite.
 * Le texte est aligné sur le bord de la zone de texte à gauche et à droite,
 * sauf la dernière ligne de chaque paragraphe qui est alignée à gauche.
 * La justification se fait en ajustant les espaces entre les mots mais sans
 * modifier les espaces entre les caractères d'un même mot.
 * @~
 * @code align 0, 1, 0, 0 @endcode
 */
align_justify();

/**
 * @~english
 * Justify text horizontally in the text box.
 * Synonym for @ref align_justify.
 * @~french
 * Justifie le texte horizontalement à gauche et à droite.
 * Synonyme de @ref align_justify.
 */
align_left_justify();

/**
 * @~english
 * Justify text horizontally in the text box.
 * Text is aligned both to the left and to the right of the text box,
 * except for the last line of each paragraph, which is aligned to the right.
 * Justification is obtained by inserting extra space between words, and
 * without changing the space between characters.
 * This is equivalent to:
 * @~french
 * Justifie le texte horizontalement à gauche et à droite.
 * Le texte est aligné sur le bord de la zone de texte à gauche et à droite,
 * sauf la dernière ligne de chaque paragraphe qui est alignée à droite.
 * La justification se fait en ajustant les espaces entre les mots mais sans
 * modifier les espaces entre les caractères d'un même mot.
 * Équivalent à :
 * @~
 * @code align 1, 1, 0, 0 @endcode
 */
align_right_justify();

/**
 * @~english
 * Justify text horizontally in the text box.
 * Text is aligned both to the left and to the right of the text box,
 * including the last line of each paragraph, even if this line is too short
 * to fit nicely in the horizontal space. In the extreme case where the last
 * line contains a single word, this word is aligned to the left.
 * Justification is obtained by inserting extra space between words, and
 * without changing the space between characters.
 * This is equivalent to:
 * @~french
 * Justifie le texte horizontalement à gauche et à droite.
 * Le texte est aligné sur le bord de la zone de texte à gauche et à droite,
 * y compris la dernière ligne de chaque paragraphe, même si cette ligne est
 * trop courte pour bien remplir l'espace. Dans le cas extrême où la dernière
 * ligne ne contient qu'un seul mot, celui-ci est aligné à gauche.
 * La justification se fait en ajustant les espaces entre les mots mais sans
 * modifier les espaces entre les caractères d'un même mot.
 * Équivalent à :
 * @~
 * @code align 1, 1, 0, 0 @endcode
 */
align_full_justify();

/**
 * @~english
 * Justify characters horizontally in the text box.
 * Similar to @ref align_justify, except that extra space in each line is
 * distributed evenly between characters, not just between words.
 * The last line of each paragraph is rendered aligned the left.
 * This is equivalent to:
 * @~french
 * Justifie les caractères horizontalement à gauche et à droite.
 * Similaire à @ref align_justify, sauf que l'espace supplémentaire sur chaque
 * ligne est réparti régulièrement entre les caractères, pas seulement entre
 * les mots. La dernière ligne de chaque paragraphe est alignée à gauche.
 * Équivalent à :
 * @~
 * @code align 0, 1, 1, 0 @endcode
 */
align_spread();

/**
 * @~english
 * Justify characters horizontally in the text box.
 * Like @ref align_spread, but all lines are fully justified (including the
 * last line of each paragraph).
 * This is equivalent to:
 * @~french
 * Justifie les caractères horizontalement à gauche et à droite.
 * Similaire à @ref align_spread, mais toutes les lignes sont totalement
 * alignées à gauche et à droite (y compris la dernière ligne de chaque
 * paragraphe).
 * Équivalent à :
 * @~
 * @code align 0, 1, 1, 1 @endcode
 */
align_full_spread();

/**
 * @~english
 * Sets parameters for vertical text layout.
 *
 * This primitive controls how lines of text are positioned vertically in the
 * current text box. The four parameters @p center, @p justify, @p spread
 * and @p full_justify may take any value between 0 and 1 (inclusive). Some
 * combinations have a usual meaning; see the functions listed below
 * for details.
 *
 * This function is to be called once per text box, before any text is written.
 * Otherwise, unexpected results may occur such as overlapping text or
 * otherwise incorrect layout.
 *
 * The default vertical text layout is @ref vertical_align_top.
 *
 * @~french
 * Définit les paramètres de justification verticale.
 *
 * Cette primitive contrôle comment les lignes de texte sont
 * positionnés verticalement dans la boîte de texte courante.
 * Les quatre parametres @p center, @p justify, @p spread
 * et @p full_justify sont entre 0 and 1 (inclus). Certaines combinaisons
 * ont une signification usuelle, cf. les fonctions listées ci-dessous pour
 * plus de détails.
 *
 * Cette primitive ne doit être appelée qu'une seule fois par boîte de texte,
 * avant d'écrire du texte. Sinon des résultats inattendus risquent de se
 * produire (superposition de textes, positionnement incorrect).
 *
 * L'alignement vertical par défaut est @ref vertical_align_top.
 *
 * @~
 * @see vertical_align_top, vertical_align_bottom, vertical_align_center.
 */
vertical_align(center:real, justify:real, spread:real, full_justify:real);

/**
 * @~english
 * Sets parameters for vertical text layout.
 * This is equivalent to:
 * @~french
 * Définit les paramètres de justification verticale.
 * Équivalent à :
 * @~
 * @code vertical_align center, justify, spread, 0 @endcode
 * @see vertical_align(center:real, justify:real, spread:real, full_justify:real);
 */
vertical_align(center:real, justify:real, spread:real);

/**
 * @~english
 * Sets parameters for vertical text layout.
 * This is equivalent to:
 * @~french
 * Définit les paramètres de justification verticale.
 * Équivalent à :
 * @~
 * @code vertical_align center, justify, 0, 0 @endcode
 * @see vertical_align(center:real, justify:real, spread:real, full_justify:real);
 */
vertical_align(center:real, justify:real);

/**
 * @~english
 * Sets parameters for vertical text layout.
 * This is equivalent to:
 * @~french
 * Définit les paramètres de justification verticale.
 * Équivalent à :
 * @~
 * @code vertical_align center, 0, 0, 0 @endcode
 * @see vertical_align(center:real, justify:real, spread:real, full_justify:real);
 */
vertical_align(center:real);


/**
 * @~english
 * Flush all lines of text to the top of the text box.
 * If there are not enough lines of text to fill the box vertically, empty
 * space is at the bottom of the box.
 * This is equivalent to:
 * @~french
 * Aligne les lignes de texte en haut de la boîte de texte.
 * Lorsqu'il n'y a pas assez de texte pour remplir la boîte, l'espace vide se
 * trouve en bas de la boîte.
 * Équivalent à :
 * @~
 * @code vertical_align 0, 0, 0, 0 @endcode
 */
vertical_align_top();

/**
 * @~english
 * Flush all lines of text to the bottom of the text box.
 * If there are not enough lines of text to fill the box vertically, empty
 * space is at the top of the box.
 * This is equivalent to:
 * @~french
 * Aligne les lignes de texte en bas de la boîte de texte.
 * Lorsqu'il n'y a pas assez de texte pour remplir la boîte, l'espace vide se
 * trouve en haut de la boîte.
 * Équivalent à :
 * @~
 * @code vertical_align 1, 0, 0, 0 @endcode
 */
vertical_align_bottom();


/**
 * @~english
 * Center lines of text vertically within the text box.
 * If there are not enough lines of text to fill the ox vertically, empty
 * space is equally distributed between top and bottom of the box.
 * This is equivalent to:
 * @~french
 * Centre les lignes de texte verticalement dans la boîte de texte.
 * Lorsqu'il n'y a pas assez de texte pour remplir la boîte, l'espace vide se
 * répartit également en haut et en bas.
 * Équivalent à :
 * @~
 * @code vertical_align 1, 0, 0, 0 @endcode
 */
vertical_align_center();

/**
 * @~english
 * Justify lines of text vertically within the text box.
 * This is equivalent to:
 * @~french
 * Justifie les lignes de texte verticalement dans la boîte de texte.
 * Équivalent à :
 * @~
 * @code vertical_align 0, 1, 0, 1 @endcode
 */
vertical_align_justify();

/**
 * @~english
 * Spread lines of text vertically within the text box.
 * Space is equally distributed between lines and top and bottom of the box.
 * This is equivalent to:
 * @~french
 * Distribue les lignes de texte verticalement dans la boîte de texte.
 * L'espace est également réparti entre les lignes et relativement au haut et
 * au bas de la boîte.
 * Équivalent à :
 * @~
 * @code vertical_align 0, 1, 1, 1 @endcode
 */
vertical_align_spread();

/**
 * @~english
 * Sets the word spacing factor.
 * Give a @p factor value larger than 1.0 to increase the space between words,
 * or a value smaller than 1.0 to reduce it.
 * @~french
 * Définit le facteur d'espacement des mots.
 * Une valeur de @p factor supérieure à 1.0 augmente l'espace entre les mots,
 * tandis qu'une valeur inférieure le réduit.
 */
word_spacing(factor:real);

/**
 * @~english
 * Sets the line spacing factor.
 * Give a @p factor value larger that 1.0 to increase the space between lines,
 * or a value smaller than 1.0 to reduce it.
 * @~french
 * Définit le facteur d'espacement des lignes.
 * Une valeur de @p factor supérieure à 1.0 augmente l'espace entre les lignes,
 * tandis qu'une valeur inférieure le réduit.
 */
line_spacing(factor:real);

/**
 * @~english
 * Sets the left and right margins in pixels.
 * @~french
 * Définit les marges gauche et droite en pixels.
 * @~
 * @bug [#793] The margins are ignored for first line of the first paragraph.
 */
margins(left:real, right:real);

/**
 * @~english
 * Sets the top and bottom margins in pixels.
 * @~french
 * Définit les marges haute et basse en pixels.
 */
vertical_margins(top:real, bottom:real);

/**
 * @}
 */
