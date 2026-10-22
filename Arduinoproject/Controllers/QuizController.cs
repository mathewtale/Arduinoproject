using Arduinoproject.Data;
using Arduinoproject.Models;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using System.Text.Json;

namespace Arduinoproject.Controllers
{
    public class TestRequest
    {
        public string Text { get; set; } = string.Empty;
    }

    [ApiController]
    [Route("api/[controller]")]
    public class QuizController : ControllerBase
    {
        private readonly AppDbContext _context;

        public QuizController(AppDbContext context)
        {
            _context = context;
        }

        [HttpPost]
    public async Task<IActionResult> SaveQuiz([FromBody] TriviaResponse response)
    {
        bool exists = await _context.Quizsave
            .AnyAsync(q => q.Question == response.Question);

        if (exists)
        {
            return Conflict("Already exists");
        }

        var quiz = new Quizsave
        {
            Question = response.Question,
            CorrectAnswer = response.CorrectAnswer,
            Category = response.Category,
            Answer1 = response.IncorrectAnswers[0],
            Answer2 = response.IncorrectAnswers[1],
            Answer3 = response.IncorrectAnswers[2]
        };

        _context.Quizsave.Add(quiz);
        await _context.SaveChangesAsync();

        return Ok("Saved successfully!");
    }

    [HttpPost("test")]
    public IActionResult Test([FromBody] TestRequest request)
        {
            return Ok(request.Text);
        }

    [HttpGet("test")]
    public IActionResult GetTest()
    {
        Console.WriteLine("Hello");
        return Ok("Hello");
    }

    [HttpGet("{id}")]
    public async Task<ActionResult<Quizsave>> GetQuizById(int id)
    {
        var quiz = await _context.Quizsave.FindAsync(id);

        if (quiz == null)
        {
            return NotFound();
        }

        return Ok(quiz);
    }

    [HttpGet]
    public async Task<ActionResult<List<Quizsave>>> GetAllQuizes()
    {
        var quizes = await _context.Quizsave.ToListAsync();

        return Ok(quizes);
    }

    }
}
