	/** String wildcard matching (* and ?)
	@remark 
	by M Shahid Shafiq
	(http://www.codeproject.com/KB/string/patmatch.aspx) */
	template<typename Char>
	bool pattern_match(const Char *str, const Char *pattern) {
		enum State {
			Exact,        // exact match

			Any,        // ?

			AnyRepeat    // *

		};

		const Char *s = str;
		const Char *p = pattern;
		const Char *q = 0;
		int state = 0;

		bool match = true;
		while (match && *p) {
			if (*p == '*') {
				state = AnyRepeat;
				q = p+1;
			} else if (*p == '?') state = Any;
			else state = Exact;

			if (*s == 0) break;

			switch (state) {
				case Exact:
					match = *s == *p;
					s++;
					p++;
					break;

				case Any:
					match = true;
					s++;
					p++;
					break;

				case AnyRepeat:
					match = true;
					s++;

					if (*s == *q) p++;
					break;
			}
		}

		if (state == AnyRepeat) return (*s == *q);
		else if (state == Any) return (*s == *p);
		else return match && (*s == *p);
	} 