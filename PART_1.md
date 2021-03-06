# Bowers & Wilkins: DSP Interview Challenge 2021
---

## Part 1 - System setup
### 1.1: Initial Setup
In this section you will configure the Raspberry Pi to play audio files via the HiFiBerry DSP expander board.

```
    1. Attach HifiBerry to Pi.
    2. Boot Pi and connect to internet (WIFI network)
    3. Update the Pi - Terminal:
            sudo apt-get update
            sudo apt-get upgrade
        Note: Default update from RaspberryPi OS may require to remove python-colorzero package:
            sudo apt autoremove ('Y' when prompted)
            sudo apt-get upgrade
        Reboot.

    4. Ensure Python, Git & Cmake are installed:
            sudo apt-get install python
            sudo apt-get install git
            sudo apt-get install -y git cmake
        
    5. Update /boot/config.txt - Enable SPI & I2S, replace audio driver with HifiBerry.
        Note: Config.txt is write-protected by default.
        Quick way to update it:
            Copy /boot/config.txt file to desktop
            Edit file:
                Uncomment:      dtparam=i2s=on
                Uncomment:      dtparam=spi=on
                Comment-Out:    dtparam=audio=on
                Add:            dtoverlay=hifiberry-dacplusdsp
            save.
        In Terminal:
            sudo -i
            cd /home/pi/Desktop
            scp config.txt /boot/
        Verify changes have been applied.
        Reboot.
    
    6. Verify HifiBerryDSP board is connected
            aplay -l
            
            **** List of PLAYBACK Hardware Devices ****
            card 0: sndrpihifiberry [snd_rpi_hifiberrydacplusdsp_sou], device 0: Hifiberry DAC+DSP SoundCard HiFi dacplusdsp-codec-0 [Hifiberry DAC+DSP SoundCard HiFi dacplusdsp-codec-0]
              Subdevices: 1/1
              Subdevice #0: subdevice #0

    7. Copy a wav file onto Pi.
    8. Use 'aplay' to play the wav file - ensure audio plays out of the HifiBerry (Phono/SPDIF).
        WARNING: PLAYBACK WILL BE LOUD!!
    
    9. Install & Update HifiBerry Toolkit (allows SigmaStudio & CLI to connect to the DSP):
			sudo pip3 install --upgrade hifiberrydsp
			bash <(curl https://raw.githubusercontent.com/hifiberry/hifiberry-dsp/master/install-dsptoolkit)
			
			Git Clone from: github.com/hifiberry/hifiberry-dsp
			cd into cloned hifiberry-dsp folder
			sudo pip install --upgrade .
			bash <(curl https://raw.githubusercontent.com/hifiberry/hifiberry-dsp/master/install-dsptoolkit)
			reboot

    10. Test TCP-=IP Server is running:
			sudo systemctl status sigmatcp
			Should say 'ACTIVE'
```

### 1.2: SigmaStudio Setup & Connection to RaspberryPi
In this section you will configure the DSP programming utility and ensure the HifiBerry audio path is configured.
**NOTE:** If you do not have access to a Windows PC, see Appendix for workaround instructions.
```
    1. Install SigmaStudio 4.6 (On your Windows PC)
        <https://www.analog.com/en/design-center/evaluation-hardware-and-software/software/ss_sigst_02.html>
    2. Connect ethernet cable between PC and Pi.
    3. Set PC & Pi network connections to both use a static IP addresses.
    4. Ensure PC can 'ping' the Pi. Ensure the Pi can 'ping' the PC.
    5. Open "dacdsp-v11.dspproj" project in SigmaStudio.
    6. On the "Hardware Configuration" Tab - Right-click, show "TCP-IP settings".
    7. Enter the IP address of the Pi (Port = 8086), and click 'Open Connection'
       Note:    There is no obvious SUCCESS message if communications is established.
                However there IS an error message if it fails.
    8. Click 'Link, Compile, Connect' to build the project.
	9. Use aplay on the pi to play audio, and ensure moving the Master-Volume fader effects the audio. 
    10. On the Hardware tab - Right-click on the ADAU1451.
                Self-Boot-Memory -> Write Latest Compilation Through DSP.
        This should program the DSP project into the HifiBerry DSP.
    10. "Export System Files" (You'll need these later).
```
**IMPORTANT: When setting the Ethernet connection on a Windows PC, if you also have WIFI active, you will possibly need to disable the 'Automatic Metric' setting in your advanced IP4 parameters, and instead use an Interface Metric = 1.
This makes your Windows PC direct traffic to your Ethernet port rather than attempting to talk over the wifi**

### 1.3: Command-Line-Interface Setup
In this section you will configure the CLI program to run on the Pi to control the audio volume.
```
    1. Copy the CLI_Tools folder to the Pi.
    2. Open a terminal in the CLI_Tools folder.
    3. Use the command 'make' to trigger a build of the Volume.c source file.
    4. Note: The build will fail due to a number of DELIBERATE errors.
    5. Debug volume.c.
              3 x Syntax errors
            + 1 x Logic error in setVolume() - it should be pretty obvious to find/fix.
    6. Rebuild the project to produce "dsp-volume" executable.
    7. Use aplay to playback an audio file.
    8. Run the volume control tool ("./dsp-volume xx") and ensure volume adjustment takes effect.
```
