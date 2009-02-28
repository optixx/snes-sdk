import sys
import re
text_raw = open(sys.argv[1],'r').readlines()
text = []
for l in text_raw:
  if not l.startswith(';'): text += [l.strip()]

text_opt = []
i = 0
while i < len(text):
  r = re.match('sta.b tcc__r([0-9]*)$',text[i])
  if r:
    #sys.stderr.write('looking for lda.b tcc__r' + r.groups()[0] + ' in ' + text[i+1] + '\n')
    if text[i+1] == 'lda.b tcc__r' + r.groups()[0]:
      #sys.stderr.write('found!\n')
      text_opt += [text[i]]	# keep store
      i += 2 # omit load
      continue
    if (text[i+1].startswith('ldx') or text[i+1].startswith('ldy')) and text[i+2] == 'lda.b tcc__r' + r.groups()[0]:
      text_opt += [text[i]]	# keep store
      text_opt += [text[i+1]]
      i += 3 # omit load
      continue
    if text[i+1] == 'pei (tcc__r' + r.groups()[0] + ')' and text[i+2].startswith('jsr.l '):
      text_opt += ['pha']
      i += 2
      continue
    if text[i+1] == 'pei (tcc__r' + r.groups()[0] + ')':
      text_opt += [text[i]]
      text_opt += ['pha']
      i += 2
      continue
    r1 = re.match('lda.b tcc__r([0-9]*)$',text[i+1])
    if r1:
      #sys.stderr.write('t '+text[i+2][:3]+'\n')
      if text[i+2][:3] in ['and','ora']:
        #sys.stderr.write('found in line ' + str(i) + '!\n')
        if text[i+2][3:] == '.b tcc__r' + r.groups()[0]:
          text_opt += [text[i]] # store
          text_opt += [text[i+2][:3] + '.b tcc__r' + r1.groups()[0]]
          i += 3
          continue
    if text[i+1] == 'sep #$20' and text[i+2] == 'lda.b tcc__r' + r.groups()[0]:
      text_opt += [text[i]]
      text_opt += [text[i+1]]
      i += 3 # skip load
      continue
    
  r = re.match('ldx #0', text[i])
  if r:
    r1 = re.match('lda.l (.*),x$', text[i+1])
    if r1 and not text[i+3].endswith(',x'):
      text_opt += ['lda.l ' + r1.groups()[0]]
      i += 2
      continue
    elif r1:
      text_opt += ['lda.l ' + r1.groups()[0]]
      text_opt += [text[i+2]]
      text_opt += [text[i+3].replace(',x','')]
      i += 4
      continue
  
  r = re.match('sta (.*),s$', text[i])
  if r:
    if text[i+1] == 'lda ' + r.groups()[0] + ',s':
      text_opt += [text[i]]
      i += 2 # omit load
      continue
  
  if text[i] == 'lda.w #0':
    if text[i+1].startswith('sta.b ') and text[i+2].startswith('lda'):
      text_opt += [text[i+1].replace('sta.','stz.')]
      i += 2
      continue
  
  r = re.match('adc #(.*)$',text[i])
  if r:
    r1 = re.match('sta.b (tcc__r[0-9]*)$', text[i+1])
    if r1:
      if text[i+2] == 'inc.b ' + r1.groups()[0] and text[i+3] == 'inc.b ' + r1.groups()[0]:
        text_opt += ['adc #' + r.groups()[0] + ' + 2']
        text_opt += [text[i+1]]
        i += 4
        continue
  
  text_opt += [text[i]]
  i += 1
  
for l in text_opt: print l
