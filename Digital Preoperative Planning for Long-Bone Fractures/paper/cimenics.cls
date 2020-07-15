%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Eccomas class for LaTeX files                 v1.1 Feb-23-2000 %%
%%                                                                %%
%% The regular article class commands still exist and may be used.%%
%% 10pt, 11pt and twocolumn options are disabled.                 %%
%%                                                                %%
%% \maketitle make the first page automatically                   %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\NeedsTeXFormat{LaTeX2e} \ProvidesClass{eccomas}[2000/09/11-14
ECCOMAS paper class]

\DeclareOption{10pt}{\OptionNotUsed}
\DeclareOption{11pt}{\OptionNotUsed}
\DeclareOption{twocolumn}{\OptionNotUsed}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{article}}

\ProcessOptions\relax

\LoadClass[12pt]{article}

\RequirePackage{calc} \RequirePackage{indentfirst}

%% Title
\renewcommand{\maketitle}{%
 \newpage
 \thispagestyle{plain}
\begin{minipage}[t][1cm][t]{15.5cm}
  \begin{flushright}
   \scriptsize
\vspace{-50pt}

  \end{flushright}
 \end{minipage}
 \begin{center}%
  \vspace{65pt}%
  {%3
   \fontsize{12}{14}\selectfont
   \bfseries\@title
    %\MakeUppercase{\@title}
   \par
  }%3
  %\vspace{18pt}%
 \vspace{13pt}%
   \end{center}   
   {%4
\fontsize{12}{14}\selectfont
   \bfseries\@author
   \par
  }%4  
  %\vspace{14pt}% 
  {%5
\fontsize{11}{12}\selectfont
   %\def\and{\\\vspace{14pt}}
\def\and{\\\vspace{13pt}}
   \normalfont\@address
   \par
  }%5
     {%6
\fontsize{12}{14}\selectfont
   \bfseries\@author
   \par
  }%6  
  %\vspace{14pt}% 
  {%7
\fontsize{11}{12}\selectfont
   %\def\and{\\\vspace{14pt}}
\def\and{\\\vspace{13pt}}
   \normalfont\@address
   \par
  }%7
  
 %\vspace{18pt}%

 \vspace{12pt}%
 \noindent\textbf{Abstract.\space}{\textit\@abstract}
%\noindent\textbf{Resumen.\space}{\textit\@abstract}
 \par
\vspace{18pt}%
\vspace{11pt}%
\noindent\textbf{Key words:\space}\@keywords
%\noindent\textbf{Palabras clave:\space}\@keywords
\par
 %\vspace{18pt}%
\thispagestyle{empty}
 %\vfill
 %\newpage
}% End of \@maketitle

\def\author{\@ifnextchar[{\@getshortauthor}{\@getauthor}}
\def\@getauthor#1{\gdef\@author{#1}\gdef\@shortauthor{#1}}
\def\@getshortauthor[#1]#2{\gdef\@shortauthor{#1}\gdef\@author{#2}}

\newcommand{\address}[1]{\gdef\@address{#1}}

\newcommand{\keywords}[1]{\gdef\@keywords{#1}}

\renewcommand{\abstract}[1]{\gdef\@abstract{#1}}

\newcommand{\heading}[1]{\gdef\@heading{#1}}

\def\ps@myheadings{%
    \def\@oddfoot{\normalfont\hfil\thepage\hfil}
    \let\@evenfoot\@oddfoot
    \def\@evenhead{%
        %\parbox[t][11.5pt][t]{\textwidth}{%
\parbox[t][21.5pt][t]{\textwidth}{%
        \centering
        \normalfont
        \fontsize{10}{10}\selectfont
        \@heading\\
\vspace{-.2cm}
        \rule{\textwidth}{1pt}%
     }
    }%
    \let\@oddhead\@evenhead
    \let\@mkboth\@gobbletwo
    \let\sectionmark\@gobble
    \let\subsectionmark\@gobble
}

\def\sectionLevel{1}
\def\sectionIndent{0pt}
\def\sectionSpaceBefore{12pt}
\def\sectionSpaceAfter{6pt}
\def\sectionStyle{\normalsize\bfseries\MakeUppercase}

\renewcommand{\section}{%
  \@startsection{section}{\sectionLevel}{\sectionIndent}{\sectionSpaceBefore}%
                {\sectionSpaceAfter}{\sectionStyle}}%
\def\sectionLevel{2}
\def\sectionIndent{0pt}
\def\sectionSpaceBefore{12pt}
\def\sectionSpaceAfter{6pt}
\def\sectionStyle{\normalsize\bfseries}

\renewcommand{\subsection}{%
  \@startsection{subsection}{\sectionLevel}{\sectionIndent}{\sectionSpaceBefore}%
                {\sectionSpaceAfter}{\sectionStyle}}%


\renewcommand{\@makecaption}[2]{%
  \vskip\abovecaptionskip
  \sbox\@tempboxa{\normalsize\fontsize{10}{12}\selectfont #1: #2}%
  \ifdim \wd\@tempboxa >\hsize
    \normalsize\fontsize{10}{12}\selectfont #1: #2\par
  \else
    \global \@minipagefalse
    \hb@xt@\hsize{\hfil\box\@tempboxa\hfil}%
  \fi
  \vskip\belowcaptionskip}

\renewenvironment{thebibliography}[1]
  {%\newpage
   \section*{\MakeUppercase\refname\@mkboth{\MakeUppercase\refname}{\MakeUppercase\refname}}%
   \list{\@biblabel{\@arabic\c@enumiv}}%
        {\settowidth\labelwidth{\@biblabel{#1}}%
         \leftmargin\labelwidth
         \advance\leftmargin\labelsep
         \@openbib@code
         \usecounter{enumiv}%
         \let\p@enumiv\@empty
         \renewcommand\theenumiv{\@arabic\c@enumiv}}%
   \sloppy
   \clubpenalty4000
   \@clubpenalty \clubpenalty
   \widowpenalty4000%
   \sfcode`\.\@m}
  {\def\@noitemerr
   {\@latex@warning{Empty `thebibliography' environment}}%
   \endlist}
\setlength{\oddsidemargin}{-5mm} \setlength{\textheight}{220mm} 
\setlength{\textwidth}{175mm} \setlength{\textfloatsep}{12pt}
\setlength{\topmargin}{-.5cm}
\setlength{\abovedisplayskip}{6pt}
\setlength{\belowdisplayskip}{6pt} \setlength{\parindent}{5mm}
\setlength{\abovecaptionskip}{6pt}
\setlength{\belowcaptionskip}{6pt} \setlength{\itemsep}{0pt}



\AtBeginDocument{%
  \pagestyle{myheadings}
  \maketitle
  \let\maketitle\relax
  
  \pagestyle{empty}

}



\endinput


\~na
