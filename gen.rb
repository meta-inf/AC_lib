lst = `find . -type f`.lines.map &:chomp
lst.sort!

lastsec = ""

puts "% AC_lib\n% dc\n"

lst.each { |f|
	File.open(f, "r") { |file|
		s = file.gets
		if not ((s.ascii_only?) and (/^\/\/@ / === s)) then
			STDERR.puts("Ignoring file #{f}")
		else
			sec = f.split('/')[1]
			title = s[4, 255]
			if (sec != lastsec) then
				puts "# #{sec}"
				lastsec = sec
			end
			puts "## #{title}"
			puts "```cpp"
			file.readlines.each { |l|
				puts l
			}
			puts "```"
		end
	}
}

# pandoc --toc --latex-engine=xelatex ret.pd -o ret.pdf --template=./pandoc-template.tex
