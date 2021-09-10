# Bowers & Wilkins: DSP Interview Challenge 2021
---

## Help! Everything's Gone Wrong!!
If you think everything has gone badly wrong, and you REALLY want to start again - you can re-format the uSD card with a fresh copy of Raspberry Pi OS.
This performs a factory-reset on the Raspberry Pi, and gets the system back to the state it should have been delivered to you.

**WARNING: This process can take quite a long time, so should only be used if you are REALLY stuck.**

```
    1. On Windows/Mac download the Raspberry Pi Imager software (https://www.raspberrypi.org/software/)
    2. Use the imager software to format the uSD card
        - "Raspberry Pi OS With Desktop" (NOT 'Lite' or 'Inc Recommended SW').
    3. Boot the Raspberry Pi - wait for 1st time setup (can take a while).
    4. Connect the Pi to the internet.
    5. Open a terminal and run: 'sudo apt-get update'
    6. Run: 'sudo apt-get upgrade'
        - This updates the installed Linux programs/drivers.
	7. 'sudo apt autoremove' ('Y' when prompted)
	8. Repeat: 'sudo apt-get update'
	9. Repeat: 'sudo apt-get upgrade'
	10. Reboot.
    11. Your Pi should be 'clean' and ready for a fresh attempt at the challenge.
```
