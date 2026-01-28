#ifndef TYPEWRITERTEXT_HPP
#define TYPEWRITERTEXT_HPP

#include <Wt/WText.h>
#include <Wt/WApplication.h>
#include <Wt/WString.h>

#include <string>

class TypewriterText : public Wt::WText {
public:
    struct Options {
        int speedMs;
        int delayMs;
        bool cursor;
        bool randomize;
        std::string cursorChar;
        bool keepCursorAfterDone;
        bool cursorBlink;

        Options()
            : speedMs(18)
            , delayMs(0)
            , cursor(false)
            , randomize(false)
            , cursorChar("▍")
            , keepCursorAfterDone(false)
            , cursorBlink(true)
        {}
    };

    explicit TypewriterText(const std::string& text, const Options& opt = Options())
        : Wt::WText("", Wt::TextFormat::Plain)
        , fullText_(text)
        , opt_(opt)
        , started_(false)
        , doStart_(true)
    {
        if (auto* app = Wt::WApplication::instance()) {
            app->require("js/TypewriterEffect.js");
        }
    }

    void setTypedText(const std::string& text) {
        fullText_ = text;
        setText("");
        started_ = false;
        doStart_ = true;
        repaint();
    }

protected:
    void render(Wt::WFlags<Wt::RenderFlag> flags) override
    {
        Wt::WText::render(flags);

        if (!doStart_ || started_ || !flags.test(Wt::RenderFlag::Full)) {
            return;
        }

        started_ = true;

        auto* app = Wt::WApplication::instance();
        if (!app) return;

        const std::string jsText =
            Wt::WString::fromUTF8(fullText_).jsStringLiteral();
        const std::string jsCursorChar =
            Wt::WString::fromUTF8(opt_.cursorChar).jsStringLiteral();

        const std::string js =
            "setTimeout(function(){"
            "  var el = " + jsRef() + ";"
            "  if (window.PortfolioTypewriter && el) {"
            "    window.PortfolioTypewriter.attach(el, {"
            "      text: " + jsText + ","
            "      speed: " + std::to_string(opt_.speedMs) + ","
            "      delay: " + std::to_string(opt_.delayMs) + ","
            "      randomize: " + std::string(opt_.randomize ? "true" : "false") + ","
            "      cursor: " + std::string(opt_.cursor ? "true" : "false") + ","
            "      cursorChar: " + jsCursorChar + ","
            "      cursorBlink: " + std::string(opt_.cursorBlink ? "true" : "false") + ","
            "      keepCursorAfterDone: " + std::string(opt_.keepCursorAfterDone ? "true" : "false") +
            "    });"
            "  }"
            "}, 0);";

        app->doJavaScript(js);
    }

private:
    std::string fullText_;
    Options opt_;
    bool started_;
    bool doStart_;
};

#endif // TYPEWRITERTEXT_HPP
