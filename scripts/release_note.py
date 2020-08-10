from sys import argv
ver = argv[1]
if ver.startswith('refs/tags/'):
    ver = ver.split('/')[-1]
note = ''

with open('src/docs/changelog.md') as f:
    for lines in f.read().split('\n## '):
        lines = lines.split('\n')
        if lines[0] == ver:
            print('<br>'.join(lines[1:]))