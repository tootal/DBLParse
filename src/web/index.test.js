var test = {
    data: {
        author: 'Katja Lenz',
        coauthor: 'Katja Lenz',
        title: 'The Conjunctive Complexity of Quadratic Boolean Functions.',
        author_ret: "[\n    {\n        \"authors\": [\n            \"Katja Lenz\"\n        ],\n        \"key\": \"homepages/39/2320\",\n        \"mdate\": \"2017-01-10\",\n        \"title\": \"Home Page\",\n        \"year\": \"\"\n    },\n    {\n        \"authors\": [\n            \"Simone Fries\",\n            \"Katja Lenz\"\n        ],\n        \"key\": \"conf/informatiktage/FriesL09\",\n        \"mdate\": \"2009-04-28\",\n        \"title\": \"Anwendungen in virtueller Realit&auml;t.\",\n        \"year\": \"2009\"\n    },\n    {\n        \"authors\": [\n            \"Carsten Damm\",\n            \"Katja Lenz\"\n        ],\n        \"key\": \"tr/trier/MI93-05\",\n        \"mdate\": \"2017-06-08\",\n        \"title\": \"Symmetric Functions in AC<sup>0</sup>&#091;2&#093;\",\n        \"year\": \"1993\"\n    },\n    {\n        \"authors\": [\n            \"Katja Lenz\",\n            \"Ingo Wegener\"\n        ],\n        \"key\": \"journals/tcs/LenzW91\",\n        \"mdate\": \"2017-05-28\",\n        \"title\": \"The Conjunctive Complexity of Quadratic Boolean Functions.\",\n        \"year\": \"1991\"\n    },\n    {\n        \"authors\": [\n            \"Katja Lenz\",\n            \"Ingo Wegener\"\n        ],\n        \"key\": \"conf/csl/LenzW87\",\n        \"mdate\": \"2017-05-19\",\n        \"title\": \"The Conjunctive Complexity of Quadratic Boolean Functions.\",\n        \"year\": \"1987\"\n    },\n    {\n        \"authors\": [\n            \"Katja Lenz\"\n        ],\n        \"key\": \"phd/dnb/Lenz92\",\n        \"mdate\": \"2017-01-10\",\n        \"title\": \"Die Komplexit&auml;t Boolescher Funktionen in Schaltkreisen &uuml;ber der Basis _424&#8853;,&#923;_425.\",\n        \"year\": \"1992\"\n    }\n]\n",
        coauthor_ret: '[\n    \"Ingo Wegener\",\n    \"Simone Fries\",\n    \"Carsten Damm\"\n]\n',
        title_ret: "[\n    {\n        \"authors\": [\n            \"Katja Lenz\",\n            \"Ingo Wegener\"\n        ],\n        \"ee\": \"https://doi.org/10.1016/0304-3975(91)90194-7\",\n        \"journal\": \"Theor. Comput. Sci.\",\n        \"key\": \"journals/tcs/LenzW91\",\n        \"mdate\": \"2017-05-28\",\n        \"name\": \"article\",\n        \"number\": \"2\",\n        \"pages\": \"257-268\",\n        \"title\": \"The Conjunctive Complexity of Quadratic Boolean Functions.\",\n        \"url\": \"https://dblp.uni-trier.de/db/journals/tcs/tcs81.html#LenzW91\",\n        \"volume\": \"81\",\n        \"year\": \"1991\"\n    },\n    {\n        \"authors\": [\n            \"Katja Lenz\",\n            \"Ingo Wegener\"\n        ],\n        \"booktitle\": \"CSL\",\n        \"crossref\": \"conf/csl/1987\",\n        \"ee\": \"https://doi.org/10.1007/3-540-50241-6_35\",\n        \"key\": \"conf/csl/LenzW87\",\n        \"mdate\": \"2017-05-19\",\n        \"name\": \"inproceedings\",\n        \"pages\": \"138-150\",\n        \"title\": \"The Conjunctive Complexity of Quadratic Boolean Functions.\",\n        \"url\": \"https://dblp.uni-trier.de/db/conf/csl/csl87.html#LenzW87\",\n        \"year\": \"1987\"\n    }\n]\n"
    },
    get title() {
        vm_inputs.type = 'title';
        vm_inputs.word = this.data.title;
        handleSearch(this.data.title_ret);
    },
    get author() {
        vm_inputs.type = 'author';
        vm_inputs.word = this.data.author;
        handleSearch(this.data.author_ret);
    },
    get coauthor() {
        vm_inputs.type = 'coauthor';
        vm_inputs.word = this.data.author;
        handleSearch(this.data.coauthor_ret);
    }
};