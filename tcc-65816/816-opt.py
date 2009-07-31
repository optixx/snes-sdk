#!/usr/bin/python

import sys
import re
text_raw = open(sys.argv[1],'r').readlines()
text = []
for l in text_raw:
  if not l.startswith(';'): text += [l.strip()]

def is_control(line):
  if len(line) > 0 and line[0] in 'jb+-' or line.endswith(':'): return True
  return False

totalopt = 0
opted = -1
opass = 0
while opted:
  opass += 1
  sys.stderr.write('optimization pass ' + str(opass) + ': ')
  opted = 0
  text_opt = []
  i = 0
  while i < len(text):
    r = re.match('st([axyz]).b tcc__([rf][0-9]*h?)$', text[i])
    if r:
      doopt = False
      for j in range(i+1, i+30):
        r1 = re.match('st([axyz]).b tcc__' + r.groups()[1] + '$', text[j])
        if r1:
          doopt = True
          break
        if text[j].startswith('jsr.l ') and not text[j].startswith('jsr.l tcc__'):
          doopt = True
          break
        if is_control(text[j]) or ('tcc__' + r.groups()[1]) in text[j]: break
        if r.groups()[1].endswith('h') and ('[tcc__' + r.groups()[1].rstrip('h')) in text[j]: break
      if doopt:
        i += 1 # skip redundant store
        opted += 1
        continue
    
    r = re.match('st([xy]).b tcc__([rf][0-9]*h?)$', text[i])
    if r:
      if text[i+1] == 'pei (tcc__' + r.groups()[1] + ')' and text[i+2].startswith('jsr.l '):
        text_opt += ['ph' + r.groups()[0]]
        i += 2
        opted += 1
        continue
      if text[i+1] == 'pei (tcc__' + r.groups()[1] + ')':
        text_opt += [text[i]]
        text_opt += ['ph' + r.groups()[0]]
        i += 2
        opted += 1
        continue
    
    r = re.match('sta.b tcc__([rf][0-9]*h?)$',text[i])
    if r:
      #sys.stderr.write('looking for lda.b tcc__r' + r.groups()[0] + ' in ' + text[i+1] + '\n')
      if text[i+1] == 'lda.b tcc__' + r.groups()[0]:
        #sys.stderr.write('found!\n')
        text_opt += [text[i]]	# keep store
        i += 2 # omit load
        opted += 1
        continue
      if (text[i+1].startswith('ldx') or text[i+1].startswith('ldy')) and text[i+2] == 'lda.b tcc__' + r.groups()[0]:
        text_opt += [text[i]]	# keep store
        text_opt += [text[i+1]]
        i += 3 # omit load
        opted += 1
        continue
      if text[i+1] == 'pei (tcc__' + r.groups()[0] + ')' and text[i+2].startswith('jsr.l '):
        text_opt += ['pha']
        i += 2
        opted += 1
        continue
      if text[i+1] == 'pei (tcc__' + r.groups()[0] + ')':
        text_opt += [text[i]]
        text_opt += ['pha']
        i += 2
        opted += 1
        continue
      elif text[i+1].startswith('pei ') and text[i+2] == 'pei (tcc__' + r.groups()[0] + ')':
        text_opt += [text[i+1]]
        text_opt += [text[i]]
        text_opt += ['pha']
        i += 3
        opted += 1
        continue
      if text[i+1] == 'inc.b tcc__' + r.groups()[0]:
        if text[i+2] == 'inc.b tcc__' + r.groups()[0] and text[i+3] == 'lda.b tcc__' + r.groups()[0]:
          text_opt += ['ina','ina','sta.b tcc__' + r.groups()[0]]
          i += 4
          opted += 1
          continue
        elif text[i+2] == 'lda.b tcc__' + r.groups()[0]:
          text_opt += ['ina','sta.b tcc__' + r.groups()[0]]
          i += 3
          opted += 1
          continue
        
      r1 = re.match('lda.b tcc__([rf][0-9]*)',text[i+1])
      if r1:
        #sys.stderr.write('t '+text[i+2][:3]+'\n')
        if text[i+2][:3] in ['and','ora']:
          #sys.stderr.write('found in line ' + str(i) + '!\n')
          if text[i+2][3:] == '.b tcc__' + r.groups()[0]:
            text_opt += [text[i]] # store
            text_opt += [text[i+2][:3] + '.b tcc__' + r1.groups()[0]]
            i += 3
            opted += 1
            continue
        
      if text[i+1] == 'sep #$20' and text[i+2] == 'lda.b tcc__' + r.groups()[0]:
        text_opt += [text[i]]
        text_opt += [text[i+1]]
        i += 3 # skip load
        opted += 1
        continue
      if not is_control(text[i+1]) and not ('tcc__' + r.groups()[0]) in text[i+1]:
        if text[i+2] == text[i]:
          text_opt += [text[i+1]]
          text_opt += [text[i+2]]
          i += 3 # skip first store
          opted += 1
          continue
      r1 = re.match('ld([xy]).b tcc__' + r.groups()[0], text[i+1])
      if r1:
        text_opt += [text[i]]
        text_opt += ['ta' + r1.groups()[0]]
        i += 2
        opted += 1
        continue
      if text[i+1] == 'clc':
        if text[i+2] == 'lda.b tcc__' + r.groups()[0]:
          text_opt += [text[i]]
          text_opt += [text[i+1]]
          i += 3 # skip load
          opted += 1
          continue
        r1 = re.match('lda.b tcc__(r[0-9]*)', text[i+2])
        if r1 and text[i+3] == 'adc.b tcc__' + r.groups()[0]:
          text_opt += [text[i]]
          text_opt += [text[i+1]]
          text_opt += ['adc.b tcc__' + r1.groups()[0]]
          i += 4 # skip load
          opted += 1
          continue
      if text[i+1] == 'asl.b tcc__' + r.groups()[0]:
        text_opt += ['asl a']
        text_opt += [text[i]]
        i += 2
        opted += 1
        continue
      
    r = re.match('st([xy]).b tcc__([rf][0-9]*h?)', text[i])
    if r:
      if text[i+1] == 'lda.b tcc__' + r.groups()[1] or text[i+1] == 'lda.b tcc__' + r.groups()[1] + " ; DON'T OPTIMIZE":
        text_opt += [text[i]]
        text_opt += ['t' + r.groups()[0] + 'a']
        i += 2
        opted += 1
        continue
        
    r = re.match('ldx #0', text[i])
    if r:
      r1 = re.match('lda.l (.*),x$', text[i+1])
      if r1 and not text[i+3].endswith(',x'):
        text_opt += ['lda.l ' + r1.groups()[0]]
        i += 2
        opted += 1
        continue
      elif r1:
        text_opt += ['lda.l ' + r1.groups()[0]]
        text_opt += [text[i+2]]
        text_opt += [text[i+3].replace(',x','')]
        i += 4
        opted += 1
        continue
    
    r = re.match('sta (.*),s$', text[i])
    if r:
      if text[i+1] == 'lda ' + r.groups()[0] + ',s':
        text_opt += [text[i]]
        i += 2 # omit load
        opted += 1
        continue
    
    if text[i] == 'lda.w #0':
      if text[i+1].startswith('sta.b ') and text[i+2].startswith('lda'):
        text_opt += [text[i+1].replace('sta.','stz.')]
        i += 2
        opted += 1
        continue
    elif text[i].startswith('lda.w #'):
      if text[i+1] == 'sep #$20' and text[i+2].startswith('sta ') and text[i+3] == 'rep #$20' and text[i+4].startswith('lda'):
        text_opt += ['sep #$20', text[i].replace('lda.w', 'lda.b'), text[i+2], text[i+3]]
        i += 4
        opted += 1
        continue
    
    if text[i] == 'rep #$20' and text[i+1] == 'sep #$20':
      i += 2
      opted += 1
      continue
    
    if text[i].startswith('lda.b') and not is_control(text[i+1]) and not 'a' in text[i+1] and text[i+2].startswith('lda.b'):
      text_opt += [text[i+1],text[i+2]]
      i += 3
      opted += 1
      continue
      
    r = re.match('adc #(.*)$',text[i])
    if r:
      r1 = re.match('sta.b (tcc__[fr][0-9]*)$', text[i+1])
      if r1:
        if text[i+2] == 'inc.b ' + r1.groups()[0] and text[i+3] == 'inc.b ' + r1.groups()[0]:
          text_opt += ['adc #' + r.groups()[0] + ' + 2']
          text_opt += [text[i+1]]
          i += 4
          opted += 1
          continue
    
    text_opt += [text[i]]
    i += 1
  text = text_opt
  sys.stderr.write(str(opted) + ' optimizations performed\n')
  totalopt += opted
  
for l in text_opt: print l
sys.stderr.write(str(totalopt) + ' optimizations performed in total\n')
