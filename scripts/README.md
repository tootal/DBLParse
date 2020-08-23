echo ('::set-output name=note::'
python .\scripts\release_note.py test

$note = python .\scripts\release_note.py test
$cmd_out = '::set-output name=note::' + $note
echo $cmd_out