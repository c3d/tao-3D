/*
 * This file duplicates module.xl for documentation purposes.
 */

/**
 * @defgroup Slides Themes for slides
 * @ingroup Modules
 *
 * Commands and themes to write a slide deck.
 *
 * The Slides module defines commands that you can use to define the structure
 * of your presentations: @ref slide, @ref title_slide, @ref base_slide,
 * @ref title_only_slide and more. You can write text bullets with symbols
 * like @ref ＊, @ref ‐ and  @ref ＋. \n
 *
 * The slide layout and colors are grouped by themes. Each theme as a name,
 * for instance @c "Keyboard" or @c "WhiteOnGray". Selecting a theme is just the
 * matter of using the @ref theme function with the suitable value, before using
 * the slide commands. For example:
 @code
import Slides 1.0

// Show a title slide with the default theme (black on white)
title_slide "My title slide",
    text "This shows the default theme"
// Change theme to "Keyboard"
theme "Keyboard"
slide "The Keyboard theme",
 *    * "Has a background picture"
 *    * "Is nicer than the default theme"
 @endcode
 *
 * The Slides module currently defines five themes in addition to the default
 * one, which is black text on white backround and can be selected by setting
 * @ref theme to any value that is not a valid theme name (for instance,
 * theme "Default" or, more simply, theme ""). See @ref theme for
 * details.
 *
 * In addition to these themes, the module exposes an interface so that
 * you can customize the appearance of your slides (font family, size and
 * weight; bullets; text backround; colors...)
 * @todo Document this interface.
 *
 * Slides are automatically scaled to the size of the display area.
 * Background pictures are automatically scaled to cover at least the display
 * area. A part of the background may disappear if the aspect ratio of the
 * display area is not the same as the aspect ratio of the picture.
 * All elements on the slide are drawn assuming that the slide is 1024x768
 * pixels by default. This can be adjusted with @ref set_slide_size.
 * Use @ref window_width and @ref window_height to get the size of the
 * display area. Use @ref slide_width and @ref slide_height to get the size
 * of the slide.
 *
 * Here is a more complete example showing all the slide types for some
 * themes in this module.
 * @include slides.ddd
 *
 * Here is a mosaic of all the pages generated by the above program,
 * <a href="examples/slides.ddd">slides.ddd</a>:
 * @image html slides.png "Output from slides.ddd"
 * @{
 */



/**
 * The name of the current theme.
 * The next command that creates a slide page will take into account the
 * value of this variable to select the appearance of the slide.
 * The possible values are:
 *  - "WhiteOnBlack" The default theme  with black and white reversed
 *  - "WhiteOnGray" Similar to the default theme, with white text on a gray
 *     backround
 *  - "WhiteOnPicture" and "BlackOnPicture" use a custom background picture
 *  - "Rounded" Similar to the default, with the addition of a thin black
 *     rounded rectangle around text areas
 *  - "Keyboard" A slightly more sophisticated theme, with a background
 *     picture and semi-transparent white text boxes
 *  - "Seyes" A theme that uses a handwriting font on a backround that looks
 *     like french ruled ("seyes") paper
 */
text theme = "";


/**
 * Select a new theme for all subsequent slides
 * The @a Theme is the name of the new theme to be applied.
 */
theme(Theme:text);


/**
 * Creates a slide without a title and story (only the background).
 * @a P is the page name.
 */
base_slide(P:text, Body:code);

/**
 * Creates a slide with title and picture content.
 * The slide title @a T is used as the page name.
 */
picture_slide(T:text, Body:code);

/**
 * Creates a slide with title and content.
 * The slide title @a T is used as the page name.
 */
slide(T:text, Body:code);

/**
 * Creates a slide with title and content.
 * @a P is the page name, @a T is the slide title.
 */
named_slide(P:text, T:text, Body:code);

/**
 * Creates a title slide (one unique text area).
 * @a P is the page name.
 */
title_slide(P:text, Body:code);

/**
 * Creates a slide with a title box only.
 * The slide title @a T is used as the page name.
 */
title_only_slide(T:text, Body:code);

/**
 * Creates a slide with a title box only.
 * @a P is the page name, @a T is the slide title.
 */
title_only_slide(P:text, T:text, Body:code);

/**
 * Display a first-level bullet.
 * Note that this is really a normal asterisk character (*). In this
 * documentation we are using a special unicode character due to
 * technical constraints.
 */
＊(T:text);

/**
 * Display a second-level bullet.
 * Note that this is really a normal minus character (-). In this
 * documentation we are using a special unicode character due to
 * technical constraints.
 */
‐(T:text);

/**
 * Display a third-level bullet.
 * Note that this is really a normal plus character (+). In this
 * documentation we are using a special unicode character due to
 * technical constraints.
 */
＋(T:text);

/**
 * Set the background picture
 * Select @a File as the background picture. This may override the default
 * selected by @ref theme.
 */
set_picture_background(File:text);

/**
 * @}
 */
