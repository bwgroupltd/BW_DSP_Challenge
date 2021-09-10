# Bowers & Wilkins: DSP Interview Challenge 2021
---

## Part 2 - Audio algorithm design & implementation (Biquad Filter)
In this section you should design and implement a Biquad audio filter.

If you're feeling ambitious, you could update the DSP Schematic to add a new filter block into the signal path.
However it is STRONGLY recommended you initially target ONE of the filters contained within the 'Tone Controls' section.

**Remember: While the example 'Volume Control' CLI program is written in C, you are NOT restricted to writing in C.**

```
    1. Create a program to accept the following inputs:
            Frequency, Q and Gain
    2. Generate the relevant coefficients for a standard biquad filter.
    3. Convert the filter coefficients to a suitable format, and transmit to the DSP.
	4. Play audio and ensure audio filter produces the desired effect.
```
**Hint: Cross-reference the Volume-Control example against the generated DSP headers to understand how to transmit your coefficients to the DSP**
