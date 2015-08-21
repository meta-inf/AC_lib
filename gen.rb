# Usage:
# ruby gen.rb > ret.pd
# pandoc --toc --latex-engine=xelatex ret.pd -o ret.pdf --template=./pandoc-template.tex

lst = `find . -type f|grep -v ".pd$"`.lines.map &:chomp
lst.sort!

lastsec = ""

puts "% AC-LIB\n% fed_up\n"

lst2 = []
lst.each { |f|
	File.open(f, "r") { |file|
		s = file.gets
		if not ((s.ascii_only?) and (/^\/\/@ / === s)) then
			STDERR.puts("Ignoring file #{f}")
		else
			lst2 = lst2 + [[f.split('/')[1], s[4, 255], f]]
		end
	}
}
lst2.sort!
lst2.each { |f|
	File.open(f[2], "r") { |file|
		s = file.gets
		if not ((s.ascii_only?) and (/^\/\/@ / === s)) then
			STDERR.puts("Ignoring file #{f[2]}")
		else
			sec = f[0]
			title = f[1]
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
