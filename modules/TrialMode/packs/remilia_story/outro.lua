loadBackground("data/scenario/remilia/effect/ED000.png")
loadBackground(packPath.."/ED001.png")
loadBackground(packPath.."/ED002.png")
loadAnimation(packPath.."/staffroll.lua")
--addCommand('generateGameAnimation '..packPath..'staffroll.txt')

addCommand('playBGM "data/bgm/ta06.ogg"')
addCommand("text Autumn at the Scarlet Devil Manor.")
addCommand("text This devil\'s manor, surrounded by roses,")
addCommand("text was only visited by a selected few.")
addCommand("text At least, that was the usual case.")
addCommand("clear")
addCommand("text But a rumor of an unorthodox and mysterious intruder had spread around,")
addCommand("text and made many curious inhabitants of Gensokyo come to this unusual place.")
addCommand("clear")
addCommand("text However, the mistress of this place was not bothered by the wave of visitors.")
addCommand("text On the contrary, she was quite happy to have so many guests.")

addCommand("background 1")
addCommand('text Remi   "Hello Patchouly,')
addCommand('text         still looking for an answer?"')
addCommand('text Patchy "Yes I\'m looking into all possible culprits."')
addCommand("clear")
addCommand('text Remi   "How funny,')
addCommand('text         I just had the answers to this mystery a moment ago."')
addCommand('text Patchy "Could you tell me what you found?"')
addCommand("clear")
addCommand('text Remi   "It doesn\'t matter for you to know.')
addCommand('text         Besides, I\'m curious about what kind of answers')
addCommand('text         you and all the visitors will find."')
addCommand("clear")
addCommand('text Patchy "I should have known,')
addCommand('text         at least you don\'t seem bored like this other time."')

addCommand("background 2")
addCommand('text Patchy "Does it mean you won\'t do anything about the mice invasion?"')
addCommand('text Remi   "You lack hospitality!')
addCommand('text         Why would I do anything about it anyway?')
addCommand('text         I am no guardian"')
addCommand('clear')
addCommand('text Patchy "Speaking of cats, where is Sakuya anyway?"')
addCommand('text Remi   "She is trying to take care of things,')
addCommand('text         but this time she will be disappointed."')
addCommand('clear')
addCommand('text Patchy "Too bad,')
addCommand('text         a nice cup of tea would have been perfect"')
addCommand('text Remi   "Don\'t worry,')
addCommand('text         she will get here soon enough"')
addCommand('clear')
addCommand('text Patchy "Do you even know where she is at this instant?')
addCommand('text         Won\'t you join us for tea when she comes back?"')
addCommand('text Remi   "I don\'t really know where she is and I might join you later,')
addCommand('text         but I have something to make sure now."')
addCommand('clear')
addCommand('text Patchy "Didn\'t you find the mystery?"')
addCommand('text Remi   "There is still a culprit I need to interrogate"')

addCommand('playBGM ""')
addCommand("end")
addCommand("text After Remilia left, Patchouli continued her research.")
addCommand("text But sadly for her, there were too few clues,")
addCommand("text and too many potential culprits in the land of Gensokyo.")

--addCommand('playGameAnimation '..packPath..'staffroll_generated.txt')
addCommand('playBGM "data/bgm/sr.ogg"')
addCommand('clear')
addCommand("text As for Remilia, she roamed the manor,")
addCommand("text looking for the visitors and for one peculiar youkai,")
addCommand("text that, she thought, should have the last pieces of the puzzle.")
addCommand('playAnimation')