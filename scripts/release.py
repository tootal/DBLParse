with open('docs/CHANGES.md', 'r', encoding='utf-8') as f:
    s = f.read()
    v1 = s.find('## v')
    v2 = s.find('## v', v1 + 1)
    lines = s[v1 : v2 - 1].split('\n')
    version = lines[0][4:]
    log = '\n'.join(lines[1:])
    with open('deploy/changes-' + str(version) + '.md', 'w', encoding='utf-8') as f1:
        f1.write(log)