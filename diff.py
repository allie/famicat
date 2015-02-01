test = open("test.log")
nestest = open("nestest.log")
lineCount = 0

for testLine, nestestLine in zip(test, nestest):
    lineCount += 1
    if testLine[:74] != nestestLine[:74]:
        print(testLine[:74])
        print(nestestLine[:74]+"\n")
