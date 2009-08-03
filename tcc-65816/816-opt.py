#!/usr/bin/python

import sys
import re
import os

verbose = True
if os.getenv('OPT816_QUIET'): verbose = False

# open the assembler file and put lines in array text
text_raw = open(sys.argv[1],'r').readlines()
text = []
for l in text_raw:
  if not l.startswith(';'): text += [l.strip()]

# checks if the line alters the control flow
def is_control(line):
  if len(line) > 0 and line[0] in 'jb+-' or line.endswith(':'): return True
  return False

def changes_accu(line):
  if (line[2] == 'a' and not line[:3] in ['pha','sta']) or (len(line) == 5 and line.endswith(' a')): return True
  else: return False

totalopt = 0	# total number of optimizations performed
opted = -1	# have we optimized in this pass?
opass = 0	# optimization pass counter
while opted:
  opass += 1
  if verbose: sys.stderr.write('optimization pass ' + str(opass) + ': ')
  opted = 0	# no optimizations performed
  text_opt = [] # optimized code array, will be filled in during this pass
  i = 0
  while i < len(text):
    # stores (accu/x/y/zero) to pseudo-registers
    r = re.match('st([axyz]).b tcc__([rf][0-9]*h?)$', text[i])
    if r:
      # eliminate redundant stores
      doopt = False
      for j in range(i+1, min(len(text),i+30)):
        r1 = re.match('st([axyz]).b tcc__' + r.groups()[1] + '$', text[j])
        if r1:
          doopt = True	# another store to the same pregister
          break
        if text[j].startswith('jsr.l ') and not text[j].startswith('jsr.l tcc__'):
          doopt = True	# before function call (will be clobbered anyway)
          break
        # cases in which we don't pursue optimization further
        if is_control(text[j]) or ('tcc__' + r.groups()[1]) in text[j]: break # branch or other use of the preg
        if r.groups()[1].endswith('h') and ('[tcc__' + r.groups()[1].rstrip('h')) in text[j]: break # use as a pointer
      if doopt:
        i += 1 # skip redundant store
        opted += 1
        continue
    
    # stores (x/y) to pseudo-registers
    r = re.match('st([xy]).b tcc__([rf][0-9]*h?)$', text[i])
    if r:
      # store hwreg to preg, push preg, function call -> push hwreg, function call
      if text[i+1] == 'pei (tcc__' + r.groups()[1] + ')' and text[i+2].startswith('jsr.l '):
        text_opt += ['ph' + r.groups()[0]]
        i += 2
        opted += 1
        continue
      # store hwreg to preg, push preg -> store hwreg to preg, push hwreg (shorter)
      if text[i+1] == 'pei (tcc__' + r.groups()[1] + ')':
        text_opt += [text[i]]
        text_opt += ['ph' + r.groups()[0]]
        i += 2
        opted += 1
        continue
      # store hwreg to preg, load hwreg from preg -> store hwreg to preg, transfer hwreg/hwreg (shorter)
      if text[i+1] == 'lda.b tcc__' + r.groups()[1] or text[i+1] == 'lda.b tcc__' + r.groups()[1] + " ; DON'T OPTIMIZE":
        text_opt += [text[i]]
        text_opt += ['t' + r.groups()[0] + 'a'] # FIXME: shouldn't this be marked as DON'T OPTIMIZE again?
        i += 2
        opted += 1
        continue
    
    # stores (accu only) to pseudo-registers
    r = re.match('sta.b tcc__([rf][0-9]*h?)$',text[i])
    if r:
      #sys.stderr.write('looking for lda.b tcc__r' + r.groups()[0] + ' in ' + text[i+1] + '\n')
      # store preg followed by load preg
      if text[i+1] == 'lda.b tcc__' + r.groups()[0]:
        #sys.stderr.write('found!\n')
        text_opt += [text[i]]	# keep store
        i += 2 # omit load
        opted += 1
        continue
      # store preg followed by load preg with ldx/ldy in between
      if (text[i+1].startswith('ldx') or text[i+1].startswith('ldy')) and text[i+2] == 'lda.b tcc__' + r.groups()[0]:
        text_opt += [text[i]]	# keep store
        text_opt += [text[i+1]]
        i += 3 # omit load
        opted += 1
        continue
      # store accu to preg, push preg, function call -> push accu, function call
      if text[i+1] == 'pei (tcc__' + r.groups()[0] + ')' and text[i+2].startswith('jsr.l '):
        text_opt += ['pha']
        i += 2
        opted += 1
        continue
      # store accu to preg, push preg -> store accu to preg, push accu (shorter)
      if text[i+1] == 'pei (tcc__' + r.groups()[0] + ')':
        text_opt += [text[i]]
        text_opt += ['pha']
        i += 2
        opted += 1
        continue
      # store accu to preg1, push preg2, push preg1 -> store accu to preg1, push preg2, push accu
      elif text[i+1].startswith('pei ') and text[i+2] == 'pei (tcc__' + r.groups()[0] + ')':
        text_opt += [text[i+1]]
        text_opt += [text[i]]
        text_opt += ['pha']
        i += 3
        opted += 1
        continue
      
      # convert incs/decs on pregs incs/decs on hwregs
      cont = False
      for crem in 'inc','dec':
        if text[i+1] == crem + '.b tcc__' + r.groups()[0]:
          # store to preg followed by crement on preg
          if text[i+2] == crem + '.b tcc__' + r.groups()[0] and text[i+3].startswith('lda'):
              # store to preg followed by two crements on preg
              # increment the accu first, then store it to preg
              text_opt += [crem + ' a',crem + ' a','sta.b tcc__' + r.groups()[0]]
              # a subsequent load can be omitted (the right value is already in the accu)
              if text[i+3] == 'lda.b tcc__' + r.groups()[0]: i += 4
              else: i += 3
              opted += 1
              cont = True
              break
          elif text[i+2].startswith('lda'): #text[i+2] == 'lda.b tcc__' + r.groups()[0]:
            # same thing with only one crement (FIXME: there should be a more clever way to do this...)
            text_opt += [crem + ' a','sta.b tcc__' + r.groups()[0]]
            if text[i+2] == 'lda.b tcc__' + r.groups()[0]: i += 3
            else: i += 2
            opted += 1
            cont = True
            break
      if cont: continue
      
      r1 = re.match('lda.b tcc__([rf][0-9]*)',text[i+1])
      if r1:
        #sys.stderr.write('t '+text[i+2][:3]+'\n')
        if text[i+2][:3] in ['and','ora']:
          # store to preg1, load from preg2, and/or preg1 -> store to preg1, and/or preg2
          #sys.stderr.write('found in line ' + str(i) + '!\n')
          if text[i+2][3:] == '.b tcc__' + r.groups()[0]:
            text_opt += [text[i]] # store
            text_opt += [text[i+2][:3] + '.b tcc__' + r1.groups()[0]]
            i += 3
            opted += 1
            continue
      
      # store to preg, switch to 8 bits, load from preg => skip the load
      if text[i+1] == 'sep #$20' and text[i+2] == 'lda.b tcc__' + r.groups()[0]:
        text_opt += [text[i]]
        text_opt += [text[i+1]]
        i += 3 # skip load
        opted += 1
        continue
      
      # two stores to preg without control flow or other uses of preg => skip first store
      if not is_control(text[i+1]) and not ('tcc__' + r.groups()[0]) in text[i+1]:
        if text[i+2] == text[i]:
          text_opt += [text[i+1]]
          text_opt += [text[i+2]]
          i += 3 # skip first store
          opted += 1
          continue
      
      # store hwreg to preg, load hwreg from preg -> store hwreg to preg, transfer hwreg/hwreg (shorter)
      r1 = re.match('ld([xy]).b tcc__' + r.groups()[0], text[i+1])
      if r1:
        text_opt += [text[i]]
        text_opt += ['ta' + r1.groups()[0]]
        i += 2
        opted += 1
        continue
      
      # store accu to preg then load accu from preg, with something in-between that does not alter
      # control flow or touch accu or preg => skip load
      if not (is_control(text[i+1]) or changes_accu(text[i+1]) or 'tcc__' + r.groups()[0] in text[i+1]):
        if text[i+2] == 'lda.b tcc__' + r.groups()[0]:
          text_opt += [text[i]]
          text_opt += [text[i+1]]
          i += 3 # skip load
          opted += 1
          continue
      
      # store preg1, clc, load preg2, add preg1 -> store preg1, clc, add preg2
      if text[i+1] == 'clc':
        r1 = re.match('lda.b tcc__(r[0-9]*)', text[i+2])
        if r1 and text[i+3] == 'adc.b tcc__' + r.groups()[0]:
          text_opt += [text[i]]
          text_opt += [text[i+1]]
          text_opt += ['adc.b tcc__' + r1.groups()[0]]
          i += 4 # skip load
          opted += 1
          continue
      
      # store accu to preg, asl preg => asl accu, store accu to preg
      # FIXME: is this safe? can we rely on code not making assumptions about the contents of the accu
      # after the shift?
      if text[i+1] == 'asl.b tcc__' + r.groups()[0]:
        text_opt += ['asl a']
        text_opt += [text[i]]
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
    
    if text[i] == 'sep #$20' and text[i+1].startswith('lda #') and text[i+2] == 'pha' and text[i+3].startswith('lda #') and text[i+4] == 'pha':
      text_opt += ['pea.w (' + text[i+1].split('#')[1] + ' * 256 + ' + text[i+3].split('#')[1] + ')']
      text_opt += [text[i]]
      i += 5
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
  if verbose: sys.stderr.write(str(opted) + ' optimizations performed\n')
  totalopt += opted
  
for l in text_opt: print l
if verbose: sys.stderr.write(str(totalopt) + ' optimizations performed in total\n')
