#include <iostream>

#ifndef __HAYAI_CONSOLE
#define __HAYAI_CONSOLE
namespace Hayai
{
    /// Static helper class for outputting to a terminal/console.
    class Console
    {
    public:
        /// Console text colors.
        enum TextColor
        {
            /// Default console color.
            
            /// Used for resets.
            TextDefault = 0,
            
            
            /// Black.
            
            /// @warning Avoid using black unless it is absolutely necesssary.
            TextBlack = 30,
            
            
            /// Blue.
            TextBlue = 34,


            /// Green.
            TextGreen = 32,
            
            
            /// Cyan.
            TextCyan = 36,
            

            /// Red.
            TextRed = 31,
            

            /// Purple.
            TextPurple = 35,
            

            /// Yellow.
            TextYellow = 33,
            

            /// White.
            
            /// @warning Avoid using white unless it is absolutely necessary.
            TextWhite = 37
        };
    };

    inline std::ostream& operator <<(std::ostream& stream,
                                     const Console::TextColor& color)
    {
        if (color == Console::TextDefault)
            return stream << "\033[m";
        else
            return stream << "\033[0;" << int(color) << "m";
    }
}
#endif
