#pragma once

#include "OpenKNX.h"

// Self-contained status-feedback helper for the OFM-SwitchActuator module.
namespace SwaStatus
{
    // Maximum rate at which a status value is transmitted while it keeps changing (during a fade
    // for ramped values, or while a measured value keeps crossing its change threshold).
    constexpr uint32_t SEND_RATE_MS = 1000;

    // Per-value bookkeeping (one instance per status KO).
    struct ValueState
    {
        float lastSent = 0.0f;        // last value actually transmitted (in scaled units, see sendValue)
        uint32_t cyclicTimer = 0;     // cyclic-send timer stamp
        uint32_t rateTimer = 0;       // rate-limit timer for change-driven sends
        bool wasInTransition = false; // edge detector: was the value fading on the previous loop?
        bool primed = false;          // has lastSent been written at least once?
    };

    // Status feedback for a scalar value.
    //   - A change is transmitted when it exceeds the thresholds (minChangePercent relative +
    //     minChangeAbsolute absolute, both in scaled units), rate-limited to one telegram per
    //     rateLimitMS. Pass 0 thresholds to send on any change.
    //   - For values that fade to a target (pass inTransition = true while fading), the exact
    //     final value is always transmitted once when the transition completes, even if a
    //     threshold would have suppressed it. The first intermediate value is only sent after one
    //     full rateLimitMS interval, so a fade shorter than rateLimitMS sends just the end value.
    //     Continuously measured values pass inTransition = false.
    //   - Cyclic sending (cyclicTimeMS) is independent of the above.
    //   - scale multiplies the value for change detection so small analogue values keep
    //     resolution (e.g. 1000 -> milli-units); the thresholds are expressed in those scaled units.
    //   - Non-transmitting paths refresh the internal KO value (valueNoSend) so a GroupValueRead
    //     is always correct. gateSend = false keeps the value/baseline current without bus traffic.
    template <typename T>
    void sendValue(GroupObject &ko, Dpt dpt, bool enabled, bool inTransition, T value,
                   ValueState &state, uint32_t cyclicTimeMS,
                   float minChangePercent, float minChangeAbsolute, uint32_t rateLimitMS,
                   bool gateSend = true, float scale = 1.0f)
    {
        if (!enabled)
        {
            ko.valueNoSend(value, dpt);
            return;
        }

        const float scaledValue = (float)value * scale;
        const bool justStarted = inTransition && !state.wasInTransition;
        const bool justSettled = state.wasInTransition && !inTransition;
        state.wasInTransition = inTransition;

        // Start the rate-limit clock when a transition begins, so the first intermediate value is
        // only sent after a full rateLimitMS interval: a fade shorter than rateLimitMS therefore
        // emits just the end value, while a longer fade emits an intermediate every rateLimitMS.
        if (justStarted)
            state.rateTimer = delayTimerInit();

        const bool changed = !state.primed || scaledValue != state.lastSent;
        const float diff = scaledValue >= state.lastSent ? scaledValue - state.lastSent : state.lastSent - scaledValue;

        bool sent = false;
        if (justSettled && gateSend && changed)
        {
            // guaranteed exact endpoint once a transition completes
            ko.value(value, dpt);
            sent = true;
        }
        else if (gateSend && changed && (rateLimitMS == 0 || delayCheckMillis(state.rateTimer, rateLimitMS)))
        {
            const bool changedEnough = !state.primed ||
                                       ((minChangePercent <= 0.0f || diff >= state.lastSent * minChangePercent / 100.0f) &&
                                        diff >= minChangeAbsolute);
            if (changedEnough)
            {
                ko.value(value, dpt);
                sent = true;
            }
        }

        if (sent)
        {
            state.lastSent = scaledValue;
            state.primed = true;
            state.rateTimer = delayTimerInit();
        }
        else
            ko.valueNoSend(value, dpt);

        if (cyclicTimeMS > 0 && delayCheckMillis(state.cyclicTimer, cyclicTimeMS))
        {
            if (gateSend)
                ko.value(value, dpt);
            else
                ko.valueNoSend(value, dpt);
            state.lastSent = scaledValue;
            state.primed = true;
            state.cyclicTimer = delayTimerInit();
        }
    }

    // Binary on/off status: transmit immediately when the state flips, plus optional cyclic send.
    inline void sendSwitch(GroupObject &ko, bool enabled, bool stateOn, uint32_t cyclicTimeMS, uint32_t &cyclicTimer)
    {
        if (!enabled)
            return;

        if ((bool)ko.value(DPT_Switch) != stateOn)
            ko.value(stateOn, DPT_Switch);

        if (cyclicTimeMS > 0 && delayCheckMillis(cyclicTimer, cyclicTimeMS))
        {
            ko.value(stateOn, DPT_Switch);
            cyclicTimer = delayTimerInit();
        }
    }
} // namespace SwaStatus
