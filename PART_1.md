# Bowers & Wilkins: DSP Interview Challenge 2021
---
## Part 1 - System setup
### 1.1: Initial Setup
In this section you will configure the Raspberry Pi to play audio files via the HiFiBerry DSP expander board.

```
    1. Attach HifiBerry to Pi.
    2. Connect peripherals to Pi (PSU, keyboard, mouse, monitor)
    3. Boot Pi and connect to internet (WIFI network)
    4. Update the Pi - Terminal:
            'sudo apt-get update'
            'sudo apt-get upgrade'
        Note: Default update from RaspberryPi OS requires to remove python-colorzero package:
            'sudo apt autoremove'
            'sudo apt-get upgrade'
        Reboot.

    5. Ensure Python is installed:
            'sudo apt-get install python'
    6. Ensure GIT is installed:
            'sudo apt-get install git'
    7. Ensure CMAKE is installed:
            'sudo apt-get install -y git cmake'
        
    8. Update /boot/config.txt - Enable SPI & I2S, replace audio driver with HifiBerry.
        Note: Config.txt is write-protected by default.
        Quick way to update it:
            Copy /boot/config.txt file to desktop
            Edit file:
                Uncomment:      dtparam=i2s=on
                Uncomment:      dtparam=spi=on
                Comment Out:    dtparam=audio=on
                Add:            dtoverlay=hifiberry-dacplusdsp
            save.
        In Terminal:
            sudo -i
            cd /home/pi/Desktop
            scp config.txt /boot/
        Verify changes have been applied.
        Reboot.
    
    9. Verify HifiBerryDSP board is connected
            'aplay -l'
            
            **** List of PLAYBACK Hardware Devices ****
            card 0: sndrpihifiberry [snd_rpi_hifiberrydacplusdsp_sou], device 0: Hifiberry DAC+DSP SoundCard HiFi dacplusdsp-codec-0 [Hifiberry DAC+DSP SoundCard HiFi dacplusdsp-codec-0]
              Subdevices: 1/1
              Subdevice #0: subdevice #0

    10. Copy a wav file onto Pi.
    11. Use 'aplay' to play the wav file - ensure audio plays out of the HifiBerry (Phono/SPDIF).
        WARNING: PLAYBACK WILL BE LOUD!!
    
    12. Update HifiBerry Toolkit - 'sudo pip3 install --upgrade hifiberrydsp'
    
    13. Get, build and install HifiBerry TCP-IP Server (allows SigmaStudio to connect to the DSP):
            https://github.com/bang-olufsen/create
            IMPORTANT: Use You MUST use the 'BEOCreate1' branch.

            cd SigmaDaemon
            cmake .
            make
            sudo cp sigmaTcpDaemon /usr/local/bin/beocreate-tcp
            sudo chown root /usr/local/bin/beocreate-tcp
            sudo chmod u+s /usr/local/bin/beocreate-tcp
            sudo chmod go+rx /usr/local/bin/beocreate-tcp
            cd ../SigmaClientTool
            make .
            make
            sudo cp SigmaClientTool /usr/local/bin/beocreate-client
            sudo chown root /usr/local/bin/beocreate-tcp
            sudo chmod go+rx /usr/local/bin/beocreate-tcp
            
            cat <<EOF >beocreate-tcp.service
            [Unit]
            Description=Beocreate DSP TCP deamon
            After=network.target
            
            [Service]
            Type=simple
            ExecStart=/usr/local/bin/beocreate-tcp
            StandardOutput=null
            Restart=on-failure
            
            [Install]
            WantedBy=multi-user.target
            Alias=beocreate-tcp.service
            EOF
            
            sudo mv beocreate-tcp.service /etc/systemd/system
            sudo systemctl daemon-reload

    14. Start the TCP-IP Server:
            sudo systemctl start beocreate-tcp
            
    IMPORTANT: If You reboot the Pi - You MUST restart the TCP-IP process.
                
    15. Verify TCP-IP Stack Is Running:
            sudo systemctl status beocreate-tcp
```

### 1.2: SigmaStudio Setup & Connection to RaspberryPi
In this section you will configure the DSP programming utility and ensure the HifiBerry audio path is configured.
**NOTE:** If you do not have access to a Windows PC, see Appendix for workaround instructions.
```
    1. Install SigmaStudio 4.6 (On your Windows PC)
        <https://www.analog.com/en/design-center/evaluation-hardware-and-software/software/ss_sigst_02.html>
    2. Connect ethernet cable between PC and Pi.
    3. Set PC & Pi network connections to both use a static IP addresses.
    4. Ensure PC can 'ping' the Pi.
    5. Ensure the Pi can 'ping' the PC.
    6. Open "dacdsp-v11.dspproj" project in SigmaStudio.
    7. On the "Hardware Configuration" Tab - Right-click, show "TCP-IP settings".
    8. Enter the IP address of the Pi (Port = 8086), and click 'Open Connection'
       Note:    There is no obvious SUCCESS message if communications is established.
                However there IS an error message if it fails.
    9. Click 'Link, Compile, Connect' to build the project.
    10. On the Hardware tab - Right-click on the ADAU1451.
                Self-Boot-Memory -> Write Latest Compilation Through DSP.
        This should program the DSP project into the HifiBerry DSP.
    11. "Export System Files" (You'll need these later).
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
            + 1 x Logic error (it should be pretty obvious to find).
    6. Rebuild the project to produce "dsp-volume" executable.
    7. Use aplay to playabck an audio file.
    8. Run the volume control tool ("./dsp-volume") and ensure volume adjustment takes effect.
```
