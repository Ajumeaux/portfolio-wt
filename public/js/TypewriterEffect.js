(function () {
  "use strict";

  const states = new WeakMap();

  function clearState(el) {
    const st = states.get(el);
    if (st && st.timer) clearTimeout(st.timer);
    states.delete(el);
  }

  function randInt(min, max) {
    // inclusif
    return Math.floor(Math.random() * (max - min + 1)) + min;
  }

  function attach(el, opts) {
    if (!el) return;

    const o = Object.assign(
      {
        text: "",
        speed: 18,
        delay: 0,
        randomize: false,
        cursor: false,
        cursorChar: "▍",
        cursorBlink: true,
        keepCursorAfterDone: false,
      },
      opts || {}
    );

    const reduceMotion =
      window.matchMedia &&
      window.matchMedia("(prefers-reduced-motion: reduce)").matches;

    clearState(el);

    el.textContent = "";
    const textSpan = document.createElement("span");
    textSpan.className = "tw-text";
    el.appendChild(textSpan);

    let cursorSpan = null;
    if (o.cursor) {
      cursorSpan = document.createElement("span");
      cursorSpan.className = "tw-cursor" + (o.cursorBlink ? " blink" : "");
      cursorSpan.textContent = o.cursorChar || "▍";
      el.appendChild(cursorSpan);
    }

    if (reduceMotion) {
      textSpan.textContent = String(o.text || "");
      if (cursorSpan && !o.keepCursorAfterDone) cursorSpan.remove();
      return;
    }

    const full = String(o.text || "");
    let i = 0;

    function nextDelay() {
      const base = Math.max(0, o.speed | 0);
      if (!o.randomize) return base;
      return randInt(0, base);
    }

    function step() {
      if (!document.body.contains(el)) return;

      textSpan.textContent = full.slice(0, i);

      if (i >= full.length) {
        if (cursorSpan && !o.keepCursorAfterDone) cursorSpan.remove();
        return;
      }

      i += 1;

      const st = states.get(el);
      st.timer = setTimeout(step, nextDelay());
    }

    const st = { timer: null };
    states.set(el, st);

    st.timer = setTimeout(step, Math.max(0, o.delay | 0));
  }

  window.PortfolioTypewriter = { attach };
})();
