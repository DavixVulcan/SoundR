from mido import MidiFile, tick2second, tempo2bpm
import tkinter as tk
from tkinter.filedialog import askopenfilename
from tkinter import Listbox, Text
import os
from pathlib import Path

filename = ""
trackid = 0

# =========================
# MIDI HELPER FUNCTIONS
# =========================

def get_note_name(midi_number):
    notes = ['C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B']
    octave = (midi_number // 12) - 1
    note = notes[midi_number % 12]
    return f"{note}{octave}"

def find_tempo_static(mid: MidiFile):
    for track in (mid.tracks):
        for msg in (track):
            if msg.type == 'set_tempo':
                return msg.tempo
            
# =========================
# TKINTER HELPERS
# =========================

def parse_note():
    
    text.delete(1.0, tk.END)

    notenames = []
    notelens = []
    mid = MidiFile(filename)
    textfilename = os.path.basename(filename)
    textfilename = Path(textfilename).stem

    ticks_per_beat = mid.ticks_per_beat
    tempo = find_tempo_static(mid)

    print("Selected:")
    selectlist = listbox.curselection()
    if selectlist:
        print(listbox.get(selectlist[0]))
        trackid = selectlist[0]

        track = mid.tracks[trackid]
        
        notestart = False
        for msg in track:
            if msg.type == 'note_on':
                if notestart is True:
                    text.insert(tk.END, "NOTES CANNOT OVERLAP")
                    return
                notestart = True
                if(msg.time != 0):
                    notenames.append("x")
                    notelens.append(round(tick2second(msg.time, ticks_per_beat, tempo), 2))
                notenames.append(get_note_name(msg.note))
                
            if msg.type == 'note_off':
                if msg.time != 0:
                    notelens.append(round(tick2second(msg.time, ticks_per_beat, tempo), 2))
                    notestart = False
    print(notenames)
    print(notelens)
    text.insert(tk.END, f"const char * {textfilename}[{len(notenames)}]\n{{\n")
    for i, string in enumerate(notenames):
        text.insert(tk.END, f'\t"{string}"')
        if(i != len(notenames)-1):
            text.insert(tk.END, f',')
        text.insert(tk.END, f'\n')
    text.insert(tk.END, '};\n\n')

    text.insert(tk.END, f'const int {textfilename}len = sizeof({textfilename}) / sizeof({textfilename}[0]);\n\n')

    text.insert(tk.END, f"const float {textfilename}times[{len(notelens)}]\n{{\n")
    for i, time in enumerate(notelens):
        text.insert(tk.END, f'\t{time}')
        if(i != len(notelens)-1):
            text.insert(tk.END, f',')
        text.insert(tk.END, f'\n')
    text.insert(tk.END, '};')



def open_midi_file():
    listbox.delete(0, tk.END)
    text.delete(1.0, tk.END)
    global filename 
    global trackid
    filename = askopenfilename()
    mid = MidiFile(filename)
    
    
    for i, track in enumerate(mid.tracks):
        notes = 0
        print('Track {}: {}'.format(i, track.name))
        for msg in track:
            if msg.type == 'note_on':
                notes += 1

        listbox.insert(i, track.name + "(notes: {})".format(notes) )
        
    trackbutton.config(state='normal')


# =========================
# TK OBJECTS
# =========================

root = tk.Tk()
root.geometry("800x600")
tk.Frame(root)
photo = tk.PhotoImage(file = "music.png")

root.iconphoto(True, photo)

listbox = Listbox(root, height = 10, 
                  width = 1, 
                  bg = "grey",
                  activestyle = 'dotbox', 
                  font = "Helvetica",
                  fg = "yellow")

filebutton = tk.Button(root, command=open_midi_file, text="open midi", width=1)
trackbutton = tk.Button(root, command=parse_note, text="select track", state='disabled', width=1)

text = tk.Text(root, width=1)

root.grid_columnconfigure(0, weight=1)
root.grid_columnconfigure(1, weight=2)
root.grid_rowconfigure(0, weight=1)
root.grid_rowconfigure(1, weight=4)
root.grid_rowconfigure(2, weight=1)

filebutton.grid(column=0, row=0, sticky=('N', 'S', 'E', 'W'))
listbox.grid(column=0, row=1, sticky=('N', 'S', 'E', 'W'))
trackbutton.grid(column=0, row=2, sticky=('N', 'S', 'E', 'W'))
text.grid(column=1,row=0, rowspan=3, sticky=('N', 'S', 'E', 'W'))
root.mainloop()