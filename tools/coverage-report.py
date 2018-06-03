import os
import sys

allFiles={}
currentFileCoverage = {}
currentFile = ""
filtered_coverage = open('coverage.info.filtered', 'w+')

excluded = []
for e in sys.argv[2:]:
    excluded.append(e.replace("\\", "/").lower())

included = sys.argv[1].replace("\\", "/").lower()

def is_included(filename):
    if filename.startswith(included):
        for e in excluded:
            if filename.startswith(e):
                return False
        return True
    return False

def get_cov_percentage(linecov):
    allLines = len(linecov)
    covered = 0;
    for line in linecov:
        if linecov[line] != 0:
            covered = covered + 1
    return (covered, allLines)


writeToFile = False
for l in open("coverage.info"):
    if l.startswith("SF:"):
        sourceFile = l[3:-1]
        currentFile = sourceFile.replace("\\", "/").lower()
        writeToFile = is_included(currentFile)
    elif l.startswith("DA:"):
        covline = l[3:-1]
        (linestr, covflagstr) = covline.split(",")
        line = int(linestr)
        covflag = int(covflagstr)
        currentFileCoverage[line] = covflag
    elif l.startswith("end_of_record"):
        allFiles[currentFile] = currentFileCoverage
        currentFileCoverage = {}
        currentFile = ""
    else:
        print("Unhandled directive %s" % l)
    if writeToFile:
        filtered_coverage.write(l)

filtered_coverage.close()
totalCovered = 0
totalLines = 0
nonTestCovered = 0
nonTestLines = 0
for f in allFiles:
    if is_included(f):
        (covered, lines) = get_cov_percentage(allFiles[f])
        print("{0} is covered {2} of {3} ({1}%)".format(f, (covered * 100.0 / lines), covered, lines))
        totalCovered = totalCovered + covered
        totalLines = totalLines + lines
        if not "test" in f and not "mock_" in f:
            nonTestCovered = nonTestCovered + covered
            nonTestLines = nonTestLines + lines

print("=== NON TEST COVERAGE %d OF %d %0.2f%% ===" % (nonTestCovered, nonTestLines, nonTestCovered*100.0 / nonTestLines))
print("=== TOTAL COVERAGE %d OF %d %0.2f%% ===" % (totalCovered, totalLines, totalCovered*100.0 / totalLines))
