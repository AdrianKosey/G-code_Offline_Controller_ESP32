#pragma once

#include <Arduino.h>
#include "gcode_types.h"
#include "gcode_state.h"

class GCodeParser
{
public:
    GCodeParser();

    // Processes ONE line and updates the internal state.
    // Returns the resulting command with the position now resolved to ABSOLUTE, in mm.
    GCodeCommand parseLine(const String& rawLine);

    void reset(); // returns to G90/G21/position(0,0,0) - use when starting a new file

    const GCodeState& getState() const;

private:
    GCodeState state;

    struct Word
    {
        char letter;
        float value;
    };

    static constexpr uint8_t MAX_WORDS_PER_LINE = 12;

    String stripComment(const String& line) const;
    uint8_t tokenize(const String& line, Word* words) const;
    bool hasWord(const Word* words, uint8_t count, char letter, float& value) const;
    float toMillimeters(float value) const;
};